const WebSocket = require('ws');

const ipAddress = 'localhost'; // Change this to the server's IP address if needed
const port = 8080;

// Connect to the WebSocket server
// const socket = new WebSocket(`ws://${ipAddress}:${port}`);
console.log('connecting');
// const socket = new WebSocket('ws://localhost:8080'); // local
const socket = new WebSocket('wss://cs29-mushroom-lamp-backend.onrender.com'); // prod


// Function to decode binary data to a string
function decodeMessage(message) {
    return Buffer.from(message).toString('utf8');
}

// Event: when the connection is established
socket.on('open', () => {
  console.log('Connected to WebSocket server (Receiver)');
});

// Event: when a message is received from the WebSocket server
socket.on('message', (data) => {
    const decodedMessage = decodeMessage(data);
    console.log('Received message:', decodedMessage);
});
// Event: when an error occurs
socket.on('error', (error) => {
  console.error('WebSocket error:', error);
});
