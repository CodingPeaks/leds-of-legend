var last_key;
var last_event;
var macros;

function testKeyBind(action) {
    if(action == 'keydown'){
        var fadein_delay = document.getElementById('fadein_delay').value;
        var fadeout_delay = document.getElementById('fadeout_delay').value;
        var brightness = document.getElementById('brightness').value;
        var cmd = '1;'+fadein_delay+';'+fadeout_delay+';'+brightness;
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

function addMacro(){
    var name = document.getElementById('macro_name').value;
    var key = document.getElementById('data').getAttribute('key');
    var fadein = parseInt(document.getElementById('fadein_delay').value);
    var fadeout = parseInt(document.getElementById('fadeout_delay').value);
    var brightness = parseInt(document.getElementById('brightness').value);
    var color = document.getElementById('rgb-color').innerText;
    eel.add_macro(name, key, fadein, fadeout, brightness, color)(updateMacroList);
}

function getMacroList(list){
    macros = list['Macros'];
    var rows = '';
    for(i=0;i<macros.length;i++){
        rows += '<div class="macro-li">'+macros[i]['name']+'</div>';
    }
    document.getElementById("macro-ul").innerHTML = rows;
}

function updateMacroList(){
    eel.read_config()(getMacroList);
}

function showKey(event){
    let text = 
        (event.shiftKey ? ' shiftKey' : '') +
        (event.ctrlKey ? ' ctrlKey' : '') +
        (event.altKey ? ' altKey' : '') +
        (event.metaKey ? ' metaKey' : '') +
        (event.repeat ? ' (repeat)' : '') + ' ' + event.key.toUpperCase();
        //console.log(event);
        document.getElementById('data').value = text;
}

function selectKey() {
    let text = 'Selected: '+
        (event.shiftKey ? ' shiftKey' : '') +
        (event.ctrlKey ? ' ctrlKey' : '') +
        (event.altKey ? ' altKey' : '') +
        (event.metaKey ? ' metaKey' : '') +
        (event.repeat ? ' (repeat)' : ' ') + event.key.toUpperCase();
        //console.log(event);
        document.getElementById('data').value = text;
        document.getElementById('data').setAttribute("key", event.code);
}


document.addEventListener('DOMContentLoaded', function(){ 

    eel.get_serial_ports()(listSerialPorts);
    eel.read_config()(getMacroList);

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
      document.getElementById('rgb-color').innerText = colorWheel.color.hexString; //rgbString
    });

    document.getElementById('serial-port').addEventListener('change', function() {
        eel.serial_begin(this.value);
    });

    document.getElementById('data').addEventListener('keydown', (event) => {
        console.log(event);
        event.preventDefault();
        if(event.key != last_key || event.repeat == false) {
            showKey(event);
            testKeyBind(event.type);
            last_key = event.key;
            last_event = event.type;
        }
    });

    document.getElementById('data').addEventListener('keyup', (event) => {
        if(event.type != last_event) {
            selectKey(event);
            testKeyBind(event.type);
            last_event = event.type;
        }

    });

}, false);
