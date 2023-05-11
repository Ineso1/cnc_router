import React, { useState, useEffect } from 'react';

function GcodeText() {
  const [fileContent, setFileContent] = useState('');
  const [portConnected, setPortConnected] = useState(false);

  const [sendingInProgress, setSendingInProgress] = useState(false);
  const [linesToSend, setLinesToSend] = useState([]);
  const [currentLineIndex, setCurrentLineIndex] = useState(0);

  useEffect(() => {
    // Check the port connection status when the component mounts
    window.mySerialPort.checkPortConnection();
  }, []);

  useEffect(() => {
    // Subscribe to port connection status updates
    window.mySerialPort.onPortConnectionStatus((connected) => {
      setPortConnected(connected);
    });
  }, []);

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

  const handleContinueClick = () => {
    setSendingInProgress(true);
  };

  useEffect(() => {
    if (sendingInProgress) {
      sendNextLine();
    }
  }, [sendingInProgress]);


  return (
    <div className="w-10/12 h-fit max-h-96 bg-neutral shadow-xl m-3 rounded">
      <input type="file" className="file-input file-input-bordered file-input-error w-full rounded" onChange={handleFileInputChange} />
      <label htmlFor="my-modal" className="btn w-full gap-2 bg-base-100 rounded">G Code Console</label>
      <input type="checkbox" id="my-modal" className="modal-toggle" />
      <div className="modal">
        <div className="modal-box">
          {fileContent.split('\n').map((line, index) => (
            <div className='w-full flex hover:bg-neutral' key={index}>
              <span className='w-10 pl-5 pr-10 text-primary text-left'>{index + 1}</span>
              <code className='text-primary-content'>{line}</code>
            </div>
          ))}
          <div className="modal-action">
            <button className="btn" onClick={handleContinueClick} disabled={!portConnected || !fileContent || sendingInProgress}>
              Continue
            </button>
          </div>
        </div>
      </div>
      <div className='h-fit max-h-60 overflow-auto'>
        {fileContent.split('\n').map((line, index) => (
          <div className='w-full flex hover:bg-sky-700' key={index}>
            <span className='w-10 pl-5 pr-10 text-base-100 text-left'>{index + 1}</span>
            <code className='text-primary-content'>{line}</code>
          </div>
        ))}
      </div>
      <div className='w-full h-10 flex mt-2'>
        <button className='tooltip w-10 pl-5 pr-10 text-succes text-left text-primary hover:text-accent-content hover:bg-primary rounded tooltip-secondary' data-tip="Add line">
          {">"}
        </button>
        <input className="border-sky-700 placeholder-slate-400 contrast-more:border-slate-400 contrast-more:placeholder-slate-500 flex-1" />
      </div>
      <button className='btn w-full gap-2 bg-warning text-warning-content rounded' onClick={handleSendGCodeClick} disabled={!portConnected || !fileContent || sendingInProgress}>
        Send G-Code
      </button>
    </div>
  );
  
}

export default GcodeText