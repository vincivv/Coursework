const clients = new Map();
let nextClientId = 0;
function addClient(response, userId, gameId) {
    const id = nextClientId++;
    response.writeHead(200, {
        "Content-Type": "text/event-stream",
        "Cache-Control": "no-cache",
        Connection: "keep-alive",
    });
    response.write("\n");
    clients.set(id, {
        id,
        response,
        userId,
        gameId,
    });
    return id;
}
function removeClient(id) {
    clients.delete(id);
}
function send(response, payload) {
    const message = `data: ${JSON.stringify(payload)}\n\n`;
    response.write(message);
}
function broadcast(payload) {
    for (const client of clients.values()) {
        send(client.response, payload);
    }
}
function broadcastToGame(gameId, payload) {
    for (const client of clients.values()) {
        if (client.gameId === gameId) {
            send(client.response, payload);
        }
    }
}
function broadcastToGameUser(gameId, userId, payload) {
    for (const client of clients.values()) {
        if (client.gameId === gameId && client.userId === userId) {
            send(client.response, payload);
        }
    }
}
export default {
    addClient,
    removeClient,
    broadcast,
    broadcastToGame,
    broadcastToGameUser,
};
//# sourceMappingURL=sse.js.map