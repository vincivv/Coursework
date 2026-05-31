import db from "./connections.js";
const create = async (user_id) => db.tx(async (transaction) => {
    const game = await transaction.one("INSERT INTO games DEFAULT VALUES RETURNING *");
    await transaction.none("INSERT INTO game_users (game_id, user_id) VALUES ($1, $2)", [
        game.id,
        user_id,
    ]);
    await transaction.none(`INSERT INTO uno_game_state (game_id, current_user_id, direction, draw_stack, status)
       VALUES ($1, $2, 1, 0, 'waiting')`, [game.id, user_id]);
    return game;
});
const join = async (game_id, user_id) => {
    await db.none(`INSERT INTO game_users (game_id, user_id)
     VALUES ($1, $2)
     ON CONFLICT (game_id, user_id) DO NOTHING`, [game_id, user_id]);
};
const list = async () => db.any(`SELECT 
        g.id, 
        g.status, 
        g.created_at, 
        u.email AS creator_email,
        (SELECT COUNT(*)::INT FROM game_users WHERE game_id = g.id) as player_count
        FROM games g
        JOIN game_users gu ON g.id = gu.game_id
        JOIN users u ON gu.user_id = u.id
        WHERE gu.joined_at = (
            SELECT MIN(joined_at) 
            FROM game_users 
            WHERE game_id = g.id
        )
        ORDER BY g.created_at DESC`);
export default { create, join, list };
//# sourceMappingURL=games.js.map