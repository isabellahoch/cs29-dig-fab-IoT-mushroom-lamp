import React, { useEffect, useRef } from 'react';

const WebSocketClient = ({ isLightOn }) => {
  // Create a ref to hold the WebSocket instance
  const socketRef = useRef(null);

  useEffect(() => {
    // Create a new WebSocket connection
    const socket = new WebSocket('ws://localhost:8080'); // local
    // const socket = new WebSocket('wss://cs29-mushroom-lamp-backend.onrender.com'); // production

    socketRef.current = socket;

    // Event: when the connection is established
    socket.onopen = () => {
      console.log('Connected to WebSocket server (React client)');
    };

    // Event: when an error occurs
    socket.onerror = (error) => {
      console.error('WebSocket error:', error);
    };

    // Cleanup function to close the WebSocket connection when the component unmounts
    return () => {
      socket.close();
    };
  }, []);

  useEffect(() => {
    // Function to send ON/OFF messages to the WebSocket server
    const sendLightStatus = () => {
      // Check if the WebSocket is open before sending the message
      if (socketRef.current && socketRef.current.readyState === WebSocket.OPEN) {
        const message = isLightOn ? 'ON' : 'OFF';
        console.log('Sending message:', message);
        socketRef.current.send(message);
      }
    };

    // Send ON/OFF messages whenever the button state changes
    sendLightStatus();
  }, [isLightOn]);

  return null; // This component doesn't render anything
};

export default WebSocketClient;
