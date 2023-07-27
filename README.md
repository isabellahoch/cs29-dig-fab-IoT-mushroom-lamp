# Mushroom Lamp Control with WebSockets & XIAO ESP32C3 🍄🌟

This repository contains a WebSocket-based application that allows clients to control the state of a 3D-printed mushroom lamp equipped with an LED strip. By connecting to the WebSocket server, you can interact with the lamp remotely and toggle the light ON/OFF in real-time. This was my first attempt at creating my own IoT device and framework.


## Deployment 🚀

Here are the deployment URLs for both the frontend and backend:

### Frontend (Hosted on Render)

The frontend React app is hosted on Render and accessible at the following URL:
- [https://cs29-mushroom-lamp.onrender.com](https://cs29-mushroom-lamp.onrender.com)
- Shortcut URL: [dartgo.org/lamp](https://dartgo.org/lamp)

### Backend (Hosted on Render)

The WebSocket server backend is hosted on Render as a web service and accessible at the following URL:
- [https://cs29-mushroom-lamp-backend.onrender.com](https://cs29-mushroom-lamp-backend.onrender.com)


## Xiao ESP32C3 & LED Strip 💡

The real magic happens with the Xiao ESP32C3 microcontroller, which is the heart of the 70s-inspired mushroom lamp. The microcontroller communicates with the WebSocket server over the network and accurately follows the user's light toggling commands. The LED strip inside the 3D-printed mushroom lamp changes its brightness according to the user's preferences, allowing the ability for greater control.

## Backend (backend/) 💿

The backend is built using Node.js and uses the `ws` library to implement a WebSocket server. It allows multiple clients to connect and communicate with each other. The server receives ON/OFF messages from the React frontend and broadcasts them to all connected clients, including the React receiver client.

### Installation

1. Navigate to the `backend` directory: 
```bash
cd backend
```

2. Install dependencies:
```bash
npm install
```

### Usage

To start the WebSocket server, run the following command:
```bash
npm start
```


The server will listen on port 8080 and wait for WebSocket connections from clients.

## Frontend (frontend/) 🖼

The frontend is a React application that connects to the WebSocket server. It allows users to toggle the virtual light ON/OFF using a button. The React client sends ON/OFF messages to the server, and the server broadcasts these messages to all connected clients, including the React receiver client.

### Installation

1. Navigate to the `frontend` directory:
```bash
cd frontend
```

2. Install dependencies:
```bash
npm install
```
### Usage

To start the React development server, run the following command:
```bash
npm start
```

The React app will be served on `http://localhost:3000`, and you can access it in your browser.

## Image Assets (frontend/src/assets/) 🌠

The frontend includes two image assets created using the Figma pen tool to demonstrate a visual virtual counterpart to the light: `mushroom_ON.svg` and `mushroom_OFF.svg`. These images are displayed based on the state of the virtual light (ON or OFF).

## How it Works 🔌

1. The React frontend provides a button that allows users to toggle the virtual light ON/OFF.

2. When the user clicks the button, the React client sends ON/OFF messages to the WebSocket server using the native WebSocket API.

3. The WebSocket server receives these messages and broadcasts them to all connected clients, including the XIAO ESP32C3 receiver chip.

5. The mushroom lamp state changes based on the state (ON or OFF) received from the server.
