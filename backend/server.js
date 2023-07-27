const WebSocket = require('ws');

// Create a WebSocket server instance
const wss = new WebSocket.Server({ port: 8080 });

let connectedClients = [];

// Function to broadcast a message to all connected clients
function broadcastMessage(message) {
  connectedClients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(message);
    }
  });
}

// Function to decode binary data to a string
function decodeMessage(message) {
    return Buffer.from(message).toString('utf8');
}


// Event: when a client connects to the server
wss.on('connection', (ws) => {
  console.log('New client connected');
  connectedClients.push(ws);

  // Event: when the server receives a message from a client
  ws.on('message', (message) => {
    const decodedMessage = decodeMessage(message);
    console.log('Received message from a client:', decodedMessage);
    broadcastMessage(message);
  });

  // Event: when a client disconnects
  ws.on('close', () => {
    console.log('Client disconnected');
    connectedClients = connectedClients.filter((client) => client !== ws);
  });

  // Event: when an error occurs
  ws.on('error', (error) => {
    console.error('WebSocket error:', error);
  });
});

console.log('WebSocket server started on port 8080');
