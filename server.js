const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 8080 }); // Change the port if needed

let ledState = false;
let interval;

wss.on('connection', (ws) => {
  console.log('Client connected');

  // Start sending messages to the client every 5 seconds
  interval = setInterval(() => {
    ledState = !ledState;
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