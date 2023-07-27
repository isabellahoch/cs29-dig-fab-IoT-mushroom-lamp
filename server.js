const WebSocket = require('ws');
const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const wss = new WebSocket.Server({ port: 8080 }); // Change the port if needed

let ledState = false;
let interval;

// Parse incoming JSON requests
app.use(bodyParser.json());

// POST route to change the ledState variable based on request payload
app.post('/api/toggleLed', (req, res) => {
  const { state } = req.body;

  if (typeof state === 'boolean') {
    ledState = state;
    res.status(200).json({ message: 'LED state changed successfully.' });
  } else {
    res.status(400).json({ error: 'Invalid request payload. "state" must be a boolean.' });
  }
});

wss.on('connection', (ws) => {
  console.log('Client connected');

  // Start sending messages to the client every 2 seconds
  interval = setInterval(() => {
    console.log("ledState", ledState);
    ws.send(ledState ? 'ON' : 'OFF');
  }, 2000);


  // Listen to incoming messages from clients
  ws.on('message', (message) => {
    console.log('Received message:', message);
    // You can process the message here and take actions accordingly
  });

  // Send initial message to clients on connection (optional)
  ws.send('WebSocket connected');

});





// Gracefully handle server shutdown
process.on('SIGINT', () => {
    clearInterval(interval);
    wss.close(() => {
      console.log('WebSocket server closed');
      process.exit(0);
    });
  });