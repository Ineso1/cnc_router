const { app, BrowserWindow, ipcMain } = require('electron');
const mySerialPort = require('../functions/PortCom');
const path = require('path');
const isDev = require('electron-is-dev');
const { join } = require("path");

let serialPath = "";
let friendlyName = "";
let portParser = { port: null, parser: null };
let pathDetected = false;

function checkPort(portParser) {
  mySerialPort.listSerialPorts()
    .then(([path, name]) => {
      if (!pathDetected && path !== serialPath) {
        serialPath = path;
        friendlyName = name;
        console.log("New port detected:", serialPath);
        [portParser.port, portParser.parser] = mySerialPort.setPort(serialPath, 9600);
        mainWindow.webContents.send("portStatus", true); // Send port status to the renderer process
        mainWindow.webContents.send("getFriendlyName", friendlyName);
        pathDetected = true;
      }
      console.log("Current Port Path:", serialPath);
    })
    .catch((error) => {
      console.error("Error:", error);
      mainWindow.webContents.send("portStatus", false); // Send port status to the renderer process
      mainWindow.webContents.send("getFriendlyName", "");
    })
    .finally(() => {
      if (!pathDetected) {
        setTimeout(() => checkPort(portParser), 12000);
        mainWindow.webContents.send("portStatus", false); // Send port status to the renderer process
        mainWindow.webContents.send("getFriendlyName", "");
      } else {
        startListening(portParser.parser);
        mainWindow.webContents.send("portStatus", true); // Send port status to the renderer process
        mainWindow.webContents.send("getFriendlyName", friendlyName);
      }
    });
}

function startListening(parser) {
  let portSerialData = "";
  parser.on('data', (data) => {
    portSerialData = data;
    console.log(portSerialData);
    mainWindow.webContents.send("dataSerial", portSerialData);
  });
  parser.on('error', (error) => {
    console.error("Serial port error:", error);
  });
}

function writeToPort(data) {
  if (portParser.port && portParser.port.isOpen) {
    portParser.port.write(data, (error) => {
      if (error) {
        console.error("Error writing to serial port:", error);
      } else {
        console.log("Data written to serial port:", data);
        mainWindow.webContents.send("dataWritten", data); // Send a message to the renderer process
      }
    });
  } else {
    console.error("Serial port is not open");
  }
}

// Listen for messages from the renderer process
ipcMain.on("writeToPort", (event, data) => {
  writeToPort(data);
});

ipcMain.on("checkPortConnection", () => {
  checkPort(portParser);
});

// Respond to the port friendlyName request
ipcMain.on('request-port-friendly-name', (event) => {
  event.reply('port-friendly-name', friendlyName);
});


/*  Eventos de Ventana ................................................................................... */
//Inicializacion del main frame
let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow(
    {
        width: 1500, 
        height: 680,
        minHeight: 650,
        minWidth: 900,
        // frame:false,
        show:false,
        webPreferences: {
            preload: join(__dirname, "./preload.js"),
        },
    });
  mainWindow.loadURL(isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, '../build/index.html')}`);
  mainWindow.on("ready-to-show", mainWindow.show);
  if (isDev) {
    // Abrir DevTools.
    //BrowserWindow.addDevToolsExtension('<location to your react chrome extension>');
    mainWindow.webContents.openDevTools();
  }
  mainWindow.on('closed', () => mainWindow = null);
}

app.on('ready', () => {
  createWindow();
  checkPort(portParser); // Start checking the port status
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});

//Funcion con evento antes de cerrar la aplicacion
app.on('before-quit', () => {
    console.log('.....................................')
});
/*................................................................*/
