import db from "./connections.js";
function shuffleCards(cards) {
    const shuffled = [...cards];
    for (let index = shuffled.length - 1; index > 0; index -= 1) {
        const swapIndex = Math.floor(Math.random() * (index + 1));
        const currentCard = shuffled[index];
        const swapCard = shuffled[swapIndex];
        if (!currentCard || !swapCard) {
            throw new Error("Card shuffle failed");
        }
        shuffled[index] = swapCard;
        shuffled[swapIndex] = currentCard;
    }
    return shuffled;
}
function takeStartingDiscard(deck) {
    const normalCardIndex = deck.findIndex((card) => card.color !== "wild");
    if (normalCardIndex >= 0) {
        const removedCards = deck.splice(normalCardIndex, 1);
        const firstRemovedCard = removedCards[0];
        if (firstRemovedCard) {
            return firstRemovedCard;
        }
    }
    const fallbackCard = deck.shift();
    if (!fallbackCard) {
        throw new Error("Cannot start game without cards");
    }
    return fallbackCard;
}
async function getPlayers(gameId) {
    return db.any(`SELECT u.id, u.email
     FROM game_users gu
     JOIN users u ON u.id = gu.user_id
     WHERE gu.game_id = $1
     ORDER BY gu.joined_at ASC`, [gameId]);
}
async function getPlayersForGame(gameId) {
    return getPlayers(gameId);
}
async function getStateRow(gameId) {
    const state = await db.oneOrNone(`SELECT game_id, current_user_id, current_color, direction, draw_stack, status
     FROM uno_game_state
     WHERE game_id = $1`, [gameId]);
    if (state === null) {
        throw new Error("Uno game state not found");
    }
    return state;
}
async function getPlayerStates(gameId) {
    return db.any(`SELECT
        u.id,
        u.email,
        COUNT(gc.id)::INT AS hand_count
     FROM game_users gu
     JOIN users u ON u.id = gu.user_id
     LEFT JOIN game_cards gc
        ON gc.game_id = gu.game_id
        AND gc.user_id = u.id
        AND gc.zone = 'hand'
     WHERE gu.game_id = $1
     GROUP BY u.id, u.email, gu.joined_at
     ORDER BY gu.joined_at ASC`, [gameId]);
}
async function getDiscardTop(gameId) {
    return db.oneOrNone(`SELECT
        gc.id AS game_card_id,
        c.id AS card_id,
        c.color,
        c.value,
        c.points,
        gc.position
     FROM game_cards gc
     JOIN cards c ON c.id = gc.card_id
     WHERE gc.game_id = $1
       AND gc.zone = 'discard'
     ORDER BY gc.position DESC
     LIMIT 1`, [gameId]);
}
async function getDeckCount(gameId) {
    const deckResult = await db.one(`SELECT COUNT(*) AS count
     FROM game_cards
     WHERE game_id = $1
       AND zone = 'deck'`, [gameId]);
    return Number(deckResult.count);
}
async function getViewerHand(gameId, viewerUserId) {
    if (viewerUserId === null) {
        return [];
    }
    return db.any(`SELECT
        gc.id AS game_card_id,
        c.id AS card_id,
        c.color,
        c.value,
        c.points,
        gc.position
     FROM game_cards gc
     JOIN cards c ON c.id = gc.card_id
     WHERE gc.game_id = $1
       AND gc.user_id = $2
       AND gc.zone = 'hand'
     ORDER BY gc.position ASC`, [gameId, viewerUserId]);
}
async function clearExistingGameCards(transaction, gameId) {
    await transaction.none("DELETE FROM game_cards WHERE game_id = $1", [gameId]);
}
async function dealHands(transaction, gameId, players, deck) {
    const cardsPerPlayer = 7;
    for (const player of players) {
        for (let position = 0; position < cardsPerPlayer; position += 1) {
            const card = deck.shift();
            if (!card) {
                throw new Error("Not enough cards to deal");
            }
            await transaction.none(`INSERT INTO game_cards (game_id, card_id, user_id, zone, position)
         VALUES ($1, $2, $3, 'hand', $4)`, [gameId, card.id, player.id, position]);
        }
    }
}
async function saveDeckCards(transaction, gameId, deck) {
    for (let position = 0; position < deck.length; position += 1) {
        const card = deck[position];
        if (!card) {
            throw new Error("Deck card is missing");
        }
        await transaction.none(`INSERT INTO game_cards (game_id, card_id, user_id, zone, position)
       VALUES ($1, $2, NULL, 'deck', $3)`, [gameId, card.id, position]);
    }
}
async function saveDiscardCard(transaction, gameId, discardCard) {
    await transaction.none(`INSERT INTO game_cards (game_id, card_id, user_id, zone, position)
     VALUES ($1, $2, NULL, 'discard', 0)`, [gameId, discardCard.id]);
}
async function getUnoGameState(gameId, viewerUserId) {
    const state = await getStateRow(gameId);
    const players = await getPlayerStates(gameId);
    const discardTop = await getDiscardTop(gameId);
    const deckCount = await getDeckCount(gameId);
    const hand = await getViewerHand(gameId, viewerUserId);
    return {
        game_id: state.game_id,
        status: state.status,
        current_user_id: state.current_user_id,
        current_color: state.current_color,
        direction: state.direction,
        draw_stack: state.draw_stack,
        deck_count: deckCount,
        players,
        discard_top: discardTop,
        hand,
    };
}
async function startGame(gameId) {
    return db.tx(async (transaction) => {
        const players = await getPlayers(gameId);
        if (players.length < 2) {
            throw new Error("At least two players are required to start the game");
        }
        const firstPlayer = players[0];
        if (!firstPlayer) {
            throw new Error("Unable to find first player");
        }
        const cards = await transaction.any("SELECT id, color, value, points FROM cards ORDER BY id ASC");
        if (cards.length === 0) {
            throw new Error("Cards table is empty");
        }
        const deck = shuffleCards(cards);
        const startingDiscard = takeStartingDiscard(deck);
        await clearExistingGameCards(transaction, gameId);
        await dealHands(transaction, gameId, players, deck);
        await saveDiscardCard(transaction, gameId, startingDiscard);
        await saveDeckCards(transaction, gameId, deck);
        await transaction.none(`INSERT INTO uno_game_state (
          game_id,
          current_user_id,
          current_color,
          direction,
          draw_stack,
          status
       )
       VALUES ($1, $2, $3, 1, 0, 'active')
       ON CONFLICT (game_id)
       DO UPDATE SET
          current_user_id = EXCLUDED.current_user_id,
          current_color = EXCLUDED.current_color,
          direction = EXCLUDED.direction,
          draw_stack = EXCLUDED.draw_stack,
          status = EXCLUDED.status,
          updated_at = CURRENT_TIMESTAMP`, [gameId, firstPlayer.id, startingDiscard.color]);
        await transaction.none("UPDATE games SET status = 'started' WHERE id = $1", [gameId]);
        return getUnoGameState(gameId, firstPlayer.id);
    });
}
async function getNextPlayerId(gameId, currentUserId) {
    const players = await getPlayers(gameId);
    const currentPlayerIndex = players.findIndex((player) => player.id === currentUserId);
    if (currentPlayerIndex < 0) {
        throw new Error("Current player is not part of this game");
    }
    const nextPlayer = players[(currentPlayerIndex + 1) % players.length];
    if (!nextPlayer) {
        throw new Error("Unable to determine next player");
    }
    return nextPlayer.id;
}
async function playCard(gameId, userId, gameCardId) {
    return db.tx(async (transaction) => {
        const state = await transaction.oneOrNone(`SELECT game_id, current_user_id, current_color, direction, draw_stack, status
       FROM uno_game_state
       WHERE game_id = $1
       FOR UPDATE`, [gameId]);
        if (state === null) {
            throw new Error("Uno game state not found");
        }
        if (state.status !== "active") {
            throw new Error("Game is not active");
        }
        if (state.current_user_id !== userId) {
            throw new Error("It is not your turn");
        }
        const selectedCard = await transaction.oneOrNone(`SELECT
          gc.id AS game_card_id,
          c.id AS card_id,
          c.color,
          c.value,
          c.points,
          gc.position
       FROM game_cards gc
       JOIN cards c ON c.id = gc.card_id
       WHERE gc.id = $1
         AND gc.game_id = $2
         AND gc.user_id = $3
         AND gc.zone = 'hand'`, [gameCardId, gameId, userId]);
        if (selectedCard === null) {
            throw new Error("Selected card was not found in your hand");
        }
        const discardTop = await getDiscardTop(gameId);
        const nextDiscardPosition = discardTop ? discardTop.position + 1 : 0;
        const nextPlayerId = await getNextPlayerId(gameId, userId);
        await transaction.none(`UPDATE game_cards
       SET position = position - 1
       WHERE game_id = $1
         AND user_id = $2
         AND zone = 'hand'
         AND position > $3`, [gameId, userId, selectedCard.position]);
        await transaction.none(`UPDATE game_cards
       SET user_id = NULL,
           zone = 'discard',
           position = $2
       WHERE id = $1`, [gameCardId, nextDiscardPosition]);
        await transaction.none(`UPDATE uno_game_state
       SET current_user_id = $2,
           current_color = $3,
           updated_at = CURRENT_TIMESTAMP
       WHERE game_id = $1`, [gameId, nextPlayerId, selectedCard.color]);
        return getUnoGameState(gameId, userId);
    });
}
export default {
    getUnoGameState,
    getPlayersForGame,
    startGame,
    playCard,
};
//# sourceMappingURL=uno.js.map