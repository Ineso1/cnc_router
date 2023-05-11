import React, { useState, useEffect } from 'react';

function FileLoad() {
  const [portConnected, setPortConnected] = useState(false);
  const [friendlyName, setFriendlyName] = useState('');

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

  const handleBadgeClick = () => {
    if (!portConnected) {
      window.mySerialPort.checkPortConnection();
    }
  };

  return (
    <div className="w-10/12 h-fit bg-neutral shadow-xl m-3 flex flex-col rounded items-center pb-4">
        <div className='w-full flex items-center py-2'>
          <h3 className='px-5'>State</h3>        
          <spam className={`w-full badge ${portConnected ? 'badge-success' : 'badge-error'} gap-2 hover:cursor-pointer`} onClick={handleBadgeClick}>
            {portConnected ? 'Connected' : 'Disconnected'}
          </spam>
        </div>
        <div className='w-full flex items-center'>
          <h3 className='px-5'>Name</h3>        
            <spam className="w-full gap-2 overflow-auto">
              {friendlyName}
            </spam>
        </div>
        <div className='w-full flex items-center'>
          <h3 className='px-5'>-</h3>        
          <spam className="w-full badge gap-2">
            -
          </spam>
        </div>
        <div className='w-full flex items-center'>
          <h3 className='px-5'>-</h3>        
          <spam className="w-full badge gap-2">
            -
          </spam>
        </div>
    </div>
  )
}

export default FileLoad