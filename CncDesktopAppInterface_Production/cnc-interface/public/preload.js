const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("mySerialPort", {
  readMyPort: (callback) => {
    ipcRenderer.on("dataSerial", (event, args) => {
      callback(args);
    });
  },
  writeToPort: (data) => {
    ipcRenderer.send('writeToPort', data);
    
    ipcRenderer.on('dataWritten', (_, data) => {
      console.log('Data written:', data); // Handle the event when data is written by the main process
    });
    
    ipcRenderer.on('lineSent', () => {
      sendNextLine(); // The main process acknowledges that a line has been sent
    });
  },
  checkPortConnection: () => {
    ipcRenderer.send('checkPortConnection');  //Render request port connection
  },
  onPortConnectionStatus: (callback) => {
    ipcRenderer.on('portStatus', (_, isConnected) => {  //sent port status
      callback(isConnected);
    });
  },
  getFriendlyName: (callback) => {
    ipcRenderer.on('getFriendlyName', (_, friendlyName) => {
      callback(friendlyName);
    });
  },
});

// Remove any existing listeners when the window is closed
window.addEventListener('beforeunload', () => {
  ipcRenderer.removeAllListeners('dataSerial');
  ipcRenderer.removeAllListeners('portStatus');
  ipcRenderer.removeAllListeners('port-friendly-name');
});

// Request the port connection status from main process
ipcRenderer.send('request-port-connection-status');

// Request the port friendlyName from main process
ipcRenderer.send('request-port-friendly-name');








