const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

//Lectura serial


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
              resolve(""); 
            } else {
              let path = ports[0].path;
              let friendlyName = ports[0].friendlyName;
              resolve([path, friendlyName]);
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
    let port = new SerialPort({ path: portPath, baudRate: baudRate, autoOpen: true});  // COM3 9600 baudios
    let parser = port.pipe(new ReadlineParser({ delimiter: '\n' }))
    console.log("Conected to" + port.path);
    return [port, parser]
}

function writePort(text){
    parser.write(text);
}

module.exports = { writePort, setPort, listSerialPorts};