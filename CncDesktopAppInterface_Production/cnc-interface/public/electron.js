const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const isDev = require('electron-is-dev');
const { join } = require("path");
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

let serialPath = "";
let friendlyName = "";
let portParser = { port: null, parser: null };
let pathDetected = false;


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
    // Open DevTools on developer mode.
    //BrowserWindow.addDevToolsExtension('<location to your react chrome extension>');
    mainWindow.webContents.openDevTools();
  }
  mainWindow.on('closed', () => mainWindow = null);
}

app.on('ready', () => {
  createWindow();
  checkPort(portParser); // Start checking the port status when ready
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

//Event before closing the app
app.on('before-quit', () => {
    console.log('.....................................')
});
/*....................................................................................................... */
/*  Port functions and settings */

async function listSerialPorts() {
  return new Promise((resolve, reject) => {
    SerialPort.list()
      .then((ports, err) => {
        if (err) {
          console.log("Error ports:", err);
          reject(err);
        } else {
          if (ports.length === 0) {
            console.log("No ports available");
            resolve([]); 
          } else {
            resolve(ports);
          }
        }
      })
      .catch((err) => {
        console.log("Error ports:", err);
        reject(err);
      });
  });
}

function setPort(portPath, baudRate){
  let port = new SerialPort({ path: portPath, baudRate: baudRate, autoOpen: true});
  let parser = port.pipe(new ReadlineParser({ delimiter: '\n' }))
  console.log("Conected to" + port.path);
  return [port, parser]
}

function checkPort(portParser) {
  listSerialPorts()
    .then((ports) => {
      let path = "";
      let name = "";
      if(ports.length !== 0){
        path = ports[0].path;
        name = ports[0].friendlyName;
      } else {
        path = "";
        name = "";
        pathDetected = false;
      }
      if (!pathDetected && path !== serialPath) {
        serialPath = path;
        friendlyName = name;
        console.log("New port detected:", serialPath);
        [portParser.port, portParser.parser] = setPort(serialPath, 9600);
        mainWindow.webContents.send("portStatus", true); // Send port status to the renderer process (boolean)
        mainWindow.webContents.send("getFriendlyName", friendlyName); // Send port name to the renderer process (str)
        pathDetected = true;
      }
      console.log("Current Port Path:", serialPath);
    })
    .catch((error) => {
      console.error("Error:", error);
      mainWindow.webContents.send("portStatus", false); // Send port status to the renderer process (boolean)
      mainWindow.webContents.send("getFriendlyName", ""); // Send port name to the renderer process (str)
    })
    .finally(() => {
      if (!pathDetected) {
        setTimeout(() => checkPort(portParser), 12000); //Try connection every 12000 milis if not pathDetected
        mainWindow.webContents.send("portStatus", false); // Send port status to the renderer process (boolean)
        mainWindow.webContents.send("getFriendlyName", ""); // Send port name to the renderer process (str)
      } else {
        startListening(portParser.parser);  //If port connected start listening port
        mainWindow.webContents.send("portStatus", true); // Send port status to the renderer process (boolean)
        mainWindow.webContents.send("getFriendlyName", friendlyName); // Send port name to the renderer process (str)
      }
    });
}

function startListening(parser) {
  let portSerialData = "";
  parser.on('data', (data) => {
    portSerialData = data;
    console.log(portSerialData);
    mainWindow.webContents.send("dataSerial", portSerialData);  //When data available on port, sends to renderProcess
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
        mainWindow.webContents.send("dataWritten", data); // Send a message to the renderer process when written 
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
