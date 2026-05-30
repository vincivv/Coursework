"use strict";
(() => {
  // src/client/game.ts
  var currentGameState = null;
  var gameIdInput = document.querySelector("#game-id");
  var gameStatus = document.querySelector("#game-status");
  var playersList = document.querySelector("#players-list");
  var discardPile = document.querySelector("#discard-pile");
  var playerHand = document.querySelector("#player-hand");
  var startGameButton = document.querySelector("#start-game");
  var drawCardButton = document.querySelector("#draw-card");
  var errorMessage = document.querySelector("#error-message");
  var gameMessage = document.querySelector("#game-message");
  var gameId = gameIdInput ? Number(gameIdInput.value) : 0;
  function setText(element, text) {
    if (element) {
      element.textContent = text;
    }
  }
  function showError(message) {
    if (!errorMessage) {
      return;
    }
    errorMessage.textContent = message;
    setTimeout(() => {
      errorMessage.textContent = "";
    }, 3e3);
  }
  function showMessage(message) {
    setText(gameMessage, message);
  }
  function formatCard(card) {
    return `${card.color} ${card.value}`;
  }
  function parseGameMessage(rawData) {
    const parsedData = JSON.parse(rawData);
    if (typeof parsedData !== "object" || parsedData === null) {
      return null;
    }
    return parsedData;
  }
  function renderPlayers(players, currentUserId) {
    if (!playersList) {
      return;
    }
    playersList.innerHTML = "";
    for (const player of players) {
      const playerRow = document.createElement("p");
      const turnMarker = currentUserId === player.id ? " \u2190 current turn" : "";
      playerRow.textContent = `${player.email}: ${String(player.hand_count)} card(s)${turnMarker}`;
      playersList.appendChild(playerRow);
    }
  }
  function renderHand(hand) {
    if (!playerHand) return;
    playerHand.innerHTML = "";
    if (hand.length === 0) {
      playerHand.textContent = "No cards in your hand.";
      return;
    }
    for (const card of hand) {
      const cardButton = document.createElement("button");
      cardButton.type = "button";
      cardButton.textContent = formatCard(card);
      cardButton.style.margin = "5px";
      cardButton.style.padding = "10px";
      cardButton.addEventListener("click", () => {
        void playCard(card.game_card_id);
      });
      playerHand.appendChild(cardButton);
    }
  }
  function renderGameState(state) {
    currentGameState = state;
    if (state.status === "finished") {
      const winner = state.players.find((player) => player.hand_count === 0);
      setText(gameStatus, `Game Over! Winner: ${winner?.email ?? "Unknown Player"}`);
    } else {
      const colorText = state.current_color ?? "none";
      setText(
        gameStatus,
        `Status: ${state.status}. Color: ${colorText}. Deck: ${String(state.deck_count)}. Stack: ${String(state.draw_stack)}`
      );
    }
    if (state.discard_top) {
      const isWild = state.discard_top.color === "wild";
      const displayColor = isWild ? state.current_color ?? "wild" : state.discard_top.color;
      setText(discardPile, `${displayColor} ${state.discard_top.value}`);
    }
    renderPlayers(state.players, state.current_user_id);
    renderHand(state.hand);
  }
  async function loadGameState() {
    const response = await fetch(`/api/games/${String(gameId)}/state`);
    if (!response.ok) return;
    const { state } = await response.json();
    renderGameState(state);
  }
  async function startGame() {
    const response = await fetch(`/api/games/${String(gameId)}/start`, {
      method: "POST",
      headers: { "Content-Type": "application/json" }
    });
    if (!response.ok) showMessage("Failed to start game.");
  }
  async function drawCard() {
    const response = await fetch(`/api/games/${String(gameId)}/draw`, {
      method: "POST",
      headers: { "Content-Type": "application/json" }
    });
    if (!response.ok) {
      const data = await response.json();
      showError(data.error ?? "Failed to draw card");
      return;
    }
    showMessage("Card(s) drawn.");
  }
  async function playCard(gameCardId) {
    if (!currentGameState) return;
    const card = currentGameState.hand.find((c) => c.game_card_id === gameCardId);
    let chosenColor;
    if (card?.color === "wild") {
      const input = window.prompt("Choose a color: red, blue, green, or yellow");
      if (!input) return;
      const sanitized = input.toLowerCase().trim();
      const validColors = ["red", "blue", "green", "yellow"];
      if (!validColors.includes(sanitized)) {
        showError("Invalid color choice.");
        return;
      }
      chosenColor = sanitized;
    }
    const response = await fetch(`/api/games/${String(gameId)}/play`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ gameCardId, chosenColor })
    });
    if (!response.ok) {
      const errorData = await response.json();
      showError(errorData.error ?? "Invalid play.");
      return;
    }
    showMessage("Card played.");
  }
  if (Number.isInteger(gameId) && gameId > 0) {
    const source = new EventSource(`/api/sse?gameId=${String(gameId)}`);
    source.onmessage = (event) => {
      const data = parseGameMessage(String(event.data));
      if (data?.type === "game_updated" && data.state) {
        renderGameState(data.state);
      }
    };
    void loadGameState();
  }
  startGameButton?.addEventListener("click", () => {
    void startGame();
  });
  drawCardButton?.addEventListener("click", () => {
    void drawCard();
  });
})();
//# sourceMappingURL=game.js.map
