const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("mySerialPort", {
  readMyPort: (callback) => {
    ipcRenderer.on("dataSerial", (event, args) => {
      callback(args);
    });
  },
  writeToPort: (data) => {
    ipcRenderer.send('writeToPort', data);
  },
  checkPortConnection: () => {
    ipcRenderer.send('checkPortConnection');
  },
  onPortConnectionStatus: (callback) => {
    ipcRenderer.on('portStatus', (_, isConnected) => {
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
