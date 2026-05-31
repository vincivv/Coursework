import db from "./connections.js";
const existing = async (email) => {
    const user = await db.oneOrNone("SELECT id FROM users WHERE email = $1", [email]);
    return user !== null;
};
const create = async (email, passwordHash, avatar) => await db.one("INSERT INTO users (email, password_hash, gravatar_url) VALUES ($1, $2, $3) RETURNING id, email, gravatar_url, created_at", [email, passwordHash, avatar]);
const findByEmail = async (email) => await db.one("SELECT * FROM users WHERE email = $1", [email]);
export default { existing, create, findByEmail };
//# sourceMappingURL=users.js.map