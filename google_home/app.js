require('dotenv').config();

const PythonShell = require('python-shell');
const fs = require('fs');
const express = require('express');
const bodyParser= require('body-parser');
const path = require('path')
const app = express();



// Switch states held in memory
const switches = [];

// Read state from saveState.json, populate switches array
var readableStream = fs.createReadStream('saveState.json');
var data = ''

readableStream.on('data', function(chunk) {
    data+=chunk;
});

readableStream.on('end', function() {
  var parsed = JSON.parse(data);

  for (i=0;i<parsed.switches.length;i++){
    switches.push(new Switch(parsed.switches[i]))
  }
});




// Switch Model
// Expects an object:{
  // id:"sw" + number,
  // state: "on" or "off",
  // name: any name you want to display. Defaults to "switch"
// }

function Switch(switchValues){
  this.id = switchValues.id || "sw"
  this.state = switchValues.state || "off"
  this.name = switchValues.name || "switch"
  this.toggle = function(){
    (this.state === "on") ? this.setState("off") : this.setState("on");
  }
  this.setState = function(state){
    //var str = state === "on" ? onString(this.id[2]) : offString(this.id[2]);
    var str = actionSwitch(this.id, state);
    PythonShell.run(str, function (err) {
      if (!process.env.DEV){
        if (err) throw err;
      } 
    });
    this.state = state;
  }
  // Invokes setState on init to set the switch to its last recalled state.
  //this.setState(this.state);
}    

// needed due to a quirk with PythonShell
function onString(name){
  //return './public/python/sw' + number + '_on.py'
  return './public/python/' + name + '.py on'
}
function offString(name){
  return './public/python/lights-all.py off'
}
function actionSwitch(name, command){
  return './public/python/' + name + '.py ' + command;
}

// Switch Lookup
function getSwitch(string){
  return switches.filter(function(element){
    return element.id === string;
  })[0]
}

// Updates saveState.json
function saveState (){
  var formattedState = {
    switches: switches
  }
  fs.writeFile('./saveState.json', JSON.stringify(formattedState) )
}

//Server Configuration
app.use(bodyParser.urlencoded({ extended: true }))
app.use(express.static(__dirname + '/public'));

// If you have a frontend, drop it in the Public folder with an entry point of index.html
app.get('/', function(req, res){
  res.sendFile('index');
})

// Switch Routes for API
app.get('/api/switches', function(req, res){
  res.send(switches);
})

app.get('/api/switches/:id', function(req, res){
  var found = getSwitch(req.params.id);
  res.json(found);
})

app.post('/api/switches/:id', function(req, res){

// For now, uses a simple password query in the url string. 
// Example: POST to localhost:8000/API/switches/sw1?password=test
  if (req.query.password === process.env.PASS){
    var foundSwitch = getSwitch(req.params.id);
    
    foundSwitch.setState(req.query.command);

    saveState();
    console.log("postSwitch "+JSON.stringify(foundSwitch));
    res.json(foundSwitch);
  }
  else {
    console.log("invalid password")
    res.send("try again")
  }
  
})

app.listen(process.env.PORT, function(){
 console.log('Listening on port ' + process.env.PORT);
})

