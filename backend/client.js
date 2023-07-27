const WebSocket = require('ws');

const ipAddress = '10.133.161.186';
const port = 8080;

// Connect to the WebSocket server
const socket = new WebSocket(`ws://${ipAddress}:${port}`);

// Function to decode binary data to a string
function decodeMessage(message) {
    return Buffer.from(message).toString('utf8');
}

// Event: when the connection is established
socket.on('open', () => {
  console.log('Connected to WebSocket server');
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

// Event: when the connection is closed
socket.on('close', (code, reason) => {
  console.log('Connection closed:', code, reason);
});
