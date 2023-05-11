import React, { useState, useEffect } from 'react'
import Loader from '../Components/Loader';
import Header from '../Components/Header';
import CncFunctionCard from '../Components/CncFunctionCard';
import Values from '../Components/Values';
import FileLoad from '../Components/FileLoad';
import GcodeText from '../Components/GcodeText';
import Window3d from '../Components/3dRender/Window3d';

function MainPage() {

  let x = 10.15;
  let y = 3.12;
  let z = -0.87;

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
      const lines = fileContent.split('\n');
      setLinesToSend(lines);
      setCurrentLineIndex(0);
      setSendingInProgress(true);
    }
  };

  const sendNextLine = () => {
    if (currentLineIndex < linesToSend.length) {
      const line = linesToSend[currentLineIndex];
      window.mySerialPort.writeToPort(line);

      // Move to the next line
      setCurrentLineIndex((prevIndex) => prevIndex + 1);
    } else {
      // Sending completed, reset state
      setSendingInProgress(false);
      setLinesToSend([]);
      setCurrentLineIndex(0);
    }
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
                  <Window3d x={x} y={y} z={z}/>
                </div>
              </div>
              <div className='flex h-full w-full pt-20 pb-5 justify-self-end z-10'>
                  <div className='flex flex-col items-center h-full w-full col-start-5 col-span-1'>
                    <Values x={x} y={y} z={z}/>
                    <CncFunctionCard/>
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