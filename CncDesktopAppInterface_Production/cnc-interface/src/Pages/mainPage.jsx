import React, { useState, useEffect } from 'react'
import Loader from '../Components/Loader';
import Header from '../Components/Header';
import CncFunctionCard from '../Components/CncFunctionCard';
import Values from '../Components/Values';
import FileLoad from '../Components/FileLoad';
import GcodeText from '../Components/GcodeText';
import Window3d from '../Components/3dRender/Window3d';
import animateGCode from '../Components/functions/gcodeAnimator';


function MainPage() {
  const [animationCoordinates, setAnimationCoordinates] = useState([]);
  const [timeStep, setTimeStep] = useState(0.1);
  // const [x, setX] = useState(10.15);
  // const [y, setY] = useState(3.12);
  // const [z, setZ] = useState(-0.87);
  const [currentCoordinates, setCurrentCoordinates] = useState({
    x: 0,
    y: 0,
    z: 0,
  });
  


  const [loading, setLoading] = useState(false);
  const [portConnected, setPortConnected] = useState(false);
  const [friendlyName, setFriendlyName] = useState('');
  useEffect(() => {
      setLoading(true);
      setTimeout(() => {
          setLoading(false);
      }, 6000);
    }, []);

  useEffect(() => {
    // Check the port connection status when the component mounts
    window.mySerialPort.checkPortConnection();
    window.mySerialPort.getFriendlyName((name) => {
      setFriendlyName(name);
    });
  }, []);

  useEffect(() => {
    // Subscribe to port connection status updates
    window.mySerialPort.onPortConnectionStatus((connected) => {
      setPortConnected(connected);
    });
  }, []);

  //Check connection on click of badge
  const handleBadgeClick = () => {
    window.mySerialPort.checkPortConnection();
    window.mySerialPort.getFriendlyName((name) => {
      setFriendlyName(name);
    });
  };


  const [fileContent, setFileContent] = useState('');
  const [sendingInProgress, setSendingInProgress] = useState(false);
  const [linesToSend, setLinesToSend] = useState([]);
  const [currentLineIndex, setCurrentLineIndex] = useState(0);
  const [animationInProgress, setAnimationInProgress] = useState(false);

  const handleFileInputChange = (event) => {
    const file = event.target.files[0];
    if (file) {
      const reader = new FileReader();
      reader.onload = (e) => {
        const content = e.target.result;
        setFileContent(content);
      };
      reader.readAsText(file);
    }
  };

  const handleSendGCodeClick = () => {
    if (portConnected && fileContent) {
      const lines = fileContent.split('\r\n');
      setLinesToSend(lines);
      setCurrentLineIndex(0);
      setSendingInProgress(true);
    }
  };

  function changeVariableOverTime(coordinates, timeInterval) {
    let currentIndex = 0;
    let intervalId = null;
  
    const startChangingVariable = () => {
      return new Promise((resolve) => {
        intervalId = setInterval(() => {
          const { x, y, z } = coordinates[currentIndex];
          setCurrentCoordinates((prevCoordinates) => ({
            ...prevCoordinates,
            x,
            y,
            z,
          }));
          currentIndex++;
          console.log("ci " + currentIndex + " cl " + coordinates.length);
          if (currentIndex >= coordinates.length) {
            stopChangingVariable();
            resolve(); // Animation completed, resolve the promise
          }
        }, timeInterval);
      });
    };
  
    const stopChangingVariable = () => {
      clearInterval(intervalId);
      currentIndex = 0;
    };
  
    return {
      start: startChangingVariable,
      stop: stopChangingVariable,
    };
  }

  // const delay = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
  //No recursivo -> (problemas)
  const sendNextLine = async () => {
    let currentIndex = currentLineIndex;
    let coordinates = { ...currentCoordinates };
  
    while (currentIndex < linesToSend.length) {
      const line = linesToSend[currentIndex];
      window.mySerialPort.writeToPort(line + "*");
  
      // Generate animation coordinates
      const animationCoords = animateGCode(coordinates.x, coordinates.y, coordinates.z, line, timeStep);
  
      if (animationCoords && animationCoords.length > 0) {
        setAnimationCoordinates(animationCoords);
        const variableChanger = changeVariableOverTime(animationCoords, timeStep * 100);
  
        // Start the animation
        setAnimationInProgress(true);
        await variableChanger.start(); // Start changing the variable
        setAnimationInProgress(false);
  
        // Update the current coordinates
        const { x, y, z } = animationCoords[animationCoords.length - 1];
        coordinates = { x, y, z };
  
        // Move to the next line
        console.log("before " + currentIndex);
        setCurrentLineIndex(currentIndex + 1); // Update the line index
  
        currentIndex++; // Increment the currentIndex manually since we are not relying on state update
      } else {
        // Invalid animation coordinates
        console.log("Invalid gcode");
        setAnimationCoordinates([]);
        // coordinates = { x: 0, y: 0, z: 0 };
  
        // Move to the next line immediately
        console.log("before " + currentIndex);
        setCurrentLineIndex(currentIndex + 1); // Update the line index
  
        currentIndex++; // Increment the currentIndex manually since we are not relying on state update
      }
    }
    window.mySerialPort.writeToPort("end*");
  
    // Update the current coordinates after sending completes
    setCurrentCoordinates(coordinates);
  
    // Sending completed, reset state
    setSendingInProgress(false);
    setLinesToSend([]);
    setCurrentLineIndex(0);
  };
  
  const [xMagnitude, setXMagnitude] = useState('');
  const [yMagnitude, setYMagnitude] = useState('');
  const [zMagnitude, setZMagnitude] = useState('');

  const handleResetCoordinates = async () => {
    const gCodeCommand = 'G0 X0 Y0 Z0';
    await sendAnimationCommand(gCodeCommand);
  };

  const handleMoveX = async () => {
    const magnitude = parseFloat(xMagnitude);
    if (!isNaN(magnitude)) {
      const gCodeCommand = `G0 X${magnitude + currentCoordinates.x} Y${currentCoordinates.y} Z${currentCoordinates.z}`;
      setXMagnitude(''); // Reset the xMagnitude state
      await sendAnimationCommand(gCodeCommand);
    }
  };
  
  const handleMoveY = async () => {
    const magnitude = parseFloat(yMagnitude);
    if (!isNaN(magnitude)) {
      const gCodeCommand = `G0 X${currentCoordinates.x} Y${magnitude + currentCoordinates.y} Z${currentCoordinates.z}`;
      setYMagnitude(''); // Reset the yMagnitude state
      await sendAnimationCommand(gCodeCommand);
    }
  };
  
  const handleMoveZ = async () => {
    const magnitude = parseFloat(zMagnitude);
    if (!isNaN(magnitude)) {
      const gCodeCommand = `G0 X${currentCoordinates.x} Y${currentCoordinates.y} Z${magnitude + currentCoordinates.z}`;
      setZMagnitude(''); // Reset the zMagnitude state
      await sendAnimationCommand(gCodeCommand);
    }
  };
  
  const sendAnimationCommand = async (gCodeCommand) => {
    let coordinates = { ...currentCoordinates };
    try {
      const animationCoords = animateGCode(
        coordinates.x,
        coordinates.y,
        coordinates.z,
        gCodeCommand,
        timeStep
      );
  
      if (animationCoords && animationCoords.length > 0) {
        setAnimationCoordinates(animationCoords);
  
        const variableChanger = changeVariableOverTime(animationCoords, timeStep * 100);
  
        setAnimationInProgress(true);
        await variableChanger.start();
        setAnimationInProgress(false);
  
        // Update the current coordinates
        const { x, y, z } = animationCoords[animationCoords.length - 1];
        coordinates = { x, y, z };
  
        console.log('Animation completed. Last position:', coordinates);
      } else {
        console.log('Animation coordinates not available.');
      }
    } catch (error) {
      console.error('Error occurred during animation:', error);
    }
    setCurrentCoordinates(coordinates);
  };
  
  
  useEffect(() => {
    if (sendingInProgress) {
      sendNextLine();
    }
  }, [sendingInProgress]);

  return (
    <div>
      {loading ? (
        <div className='flex items-center justify-center flex-row h-screen w-screen'>
            <Loader/>
        </div>
      ) : (
        <>
          <div className='h-screen w-full'>
            <Header/>
            <div className='h-full w-full grid grid-cols-4'>
              <div className='flex h-full w-full pt-20 pb-5 justify-self-start z-10'>
                  <div className='grid grid-cols-1 gap-3 h-full w-full'>
                      <FileLoad portStatus={portConnected} portName={friendlyName} reconect={handleBadgeClick}/>
                      <GcodeText fileContent={fileContent} portStatus={portConnected} sendingInProgress={sendingInProgress} handleFileInput={handleFileInputChange} handleSendGCode={handleSendGCodeClick} />
                  </div>
              </div>
              <div className='flex h-full w-full pt-20 pb-5 col-span-2'>
                <div className='absolute h-full w-full left-0'>
                  <Window3d x={currentCoordinates.x} y={currentCoordinates.y} z={currentCoordinates.z}/>
                </div>
              </div>
              <div className='flex h-full w-full pt-20 pb-5 justify-self-end z-10'>
                  <div className='flex flex-col items-center h-full w-full col-start-5 col-span-1'>
                    <Values x={currentCoordinates.x} y={currentCoordinates.y} z={currentCoordinates.z}/>
                    <CncFunctionCard autoSetCoordinates={handleResetCoordinates} moveX={handleMoveX} moveY={handleMoveY} moveZ={handleMoveZ} xMagnitude={xMagnitude} setXMagnitude={setXMagnitude} yMagnitude={yMagnitude} setYMagnitude={setYMagnitude} zMagnitude={zMagnitude} setZMagnitude={setZMagnitude}/>
                  </div>
              </div>
            </div>
          </div>
        </>
      )}
    </div>
  )
}

export default MainPage