import React, { useEffect } from "react";
import './App.css';
import MainPage from './Pages/mainPage'


function App() {
  useEffect(() => {
    window.mySerialPort.readMyPort((data) => {
      console.log("Received response from Arduino:", data);
      // Perform any necessary actions based on the received response
    });
  }, []);

  const sendMessageToArduino = () => {
    const message = "Hello from Electron!";
    window.mySerialPort.writeToPort(message);
    console.log("Sent message to Arduino:", message);
  };

  return (
    <div className="App">
      <MainPage />
      {/* <button onClick={sendMessageToArduino}>Send Message to Arduino</button> */}
    </div>
  );
}

export default App;