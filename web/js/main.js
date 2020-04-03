var last_key;
var last_event;

function bindKey(action) {
    if(action == 'keydown'){
        var cmd = '1;25;500;25';
    } else if(action == 'keyup') {
        var cmd = '0';
    }
 eel.serial_write(cmd)(callBack)
}

function callBack() {
    console.log('Key successfully binded!');
}

function listSerialPorts(ports) {
 console.log(ports);
 var options = '';
 if(ports.length > 0) {
    options += '<option value="">Select port</option>';
    for(i=0; i<ports.length;i++){
        options += '<option value="' + ports[i] + '">' + ports[i] + '</option>';
    }
 } else {
        options += '<option disabled selected>No available ports</option>';
 }
    document.getElementById('serial-port').innerHTML = options;
}

function serialPortStatus(status){
    console.log(status);
}

document.addEventListener('keydown', (event) => {
    if(event.key != last_key || event.repeat == false) {
        showKey('Pressed: ', event);
        last_key = event.key;
        last_event = event.type;
    }
});

document.addEventListener('keyup', (event) => {
    if(event.type != last_event) {
        showKey('Released: ', event);
        last_event = event.type;
    }

});


function showKey(type, event){
 let text = type + event.code +
    (event.shiftKey ? ' shiftKey' : '') +
    (event.ctrlKey ? ' ctrlKey' : '') +
    (event.altKey ? ' altKey' : '') +
    (event.metaKey ? ' metaKey' : '') +
    (event.repeat ? ' (repeat)' : '') +
    "\n";
    //console.log(event);
    document.getElementById('data').value = text;
    console.log('new key: '+event.key);
    console.log('old key: '+last_key);
    console.log('repeat: '+event.repeat);
    console.log('\n');
    
    bindKey(event.type);
    
}

function clearKey() {
    document.getElementById('data').value = '';
}

document.addEventListener('DOMContentLoaded', function(){ 

    eel.get_serial_ports()(listSerialPorts);
    //eel.init_serial_port('COM5')(serialPortStatus);


    var colorWheel = new iro.ColorPicker("#colorWheelDemo", {
        width: 270,
        layout: [
        { 
          component: iro.ui.Wheel,
          options: {
            wheelLightness: true,
            wheelAngle: 0,
            wheelDirection: "anticlockwise"
          } 
        }
        ]
      
    });

    colorWheel.on('input:change', function(color, changes){
      document.getElementById('color').style.backgroundColor = colorWheel.color.hexString;
      document.getElementById('rgb-color').innerText = colorWheel.color.rgbString;
    });

    document.getElementById('serial-port').addEventListener('change', function() {
        eel.serial_begin(this.value);
    });

}, false);

function IgnoreAlpha(e) {
  if (!e) {
    e = window.event;
  }
  if (e.keyCode >= 65 && e.keyCode <= 90) // A to Z
  {
    e.returnValue = false;
    e.cancel = true;
  }
}