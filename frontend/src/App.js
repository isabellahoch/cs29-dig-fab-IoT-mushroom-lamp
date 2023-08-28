import React, { useState } from 'react';
import './App.css';
import WebSocketClient from './WebSocketClient';

function App() {
  const [isLightOn, setLightOn] = useState(true);

  // Function to handle the button click and toggle the light status
  const handleToggleLight = () => {
    setLightOn((prevIsLightOn) => !prevIsLightOn);
  };

  return (
    <div className="App">
      <button onClick={handleToggleLight}>
        <div style={{alignItems: 'center', justifyContent: 'center', display: 'flex', flexDirection: 'column', height:'100vh'}}>
          {isLightOn ? (
            <img src={require('./assets/mushroom_ON.svg').default} alt="Mushroom ON" />
          ) : (
            <img src={require('./assets/mushroom_OFF.svg').default} alt="Mushroom OFF" />
          )}
        </div>
      </button>
      <WebSocketClient isLightOn={isLightOn} />
    </div>
  );
}

export default App;
