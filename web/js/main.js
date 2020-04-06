var last_key;
var last_event;
var macros;

function startKeyLogger(){
    eel.start_kl();
}

function stopKeyLogger(){
    eel.stop_kl();
}

function testKeyBind(action) {
    if(action == 'keydown'){
        var fadein_delay = document.getElementById('fadein_delay').value;
        var brightness = document.getElementById('brightness').value;
        var cmd = brightness+';'+fadein_delay;
    } else if(action == 'keyup') {
        var fadeout_delay = document.getElementById('fadeout_delay').value;
        var cmd = '0;'+fadeout_delay;
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

function getSerialPort(){
    eel.get_serial_port()(initSerialPort);
}

function initSerialPort(port){
    eel.serial_begin(port);
}

function addMacro(){
    var name = document.getElementById('macro_name').value;
    var key = document.getElementById('data').getAttribute('key');
    var keycode = parseInt(document.getElementById('data').getAttribute('keycode'));
    var fadein = parseInt(document.getElementById('fadein_delay').value);
    var fadeout = parseInt(document.getElementById('fadeout_delay').value);
    var brightness = parseInt(document.getElementById('brightness').value);
    var color = document.getElementById('color').innerText;
    if(fadein && fadeout &&  brightness && key && name){
    	eel.add_macro(name, key, keycode, fadein, fadeout, brightness, color)(updateMacroList);
    	//alert("Successfully saved");
	}else{
		alert("Please fill all forms before saving");
	}
}

function deleteMacro(macro_name){
    eel.delete_macro(macro_name)(updateMacroList);
}

function getMacroList(list){
    macros = list['Macros'];
    var rows = '';
    for(i=0;i<macros.length;i++){
        rows += '<div class="macro-li">'+macros[i]['name']+'<i title="Delete" onclick="deleteMacro(\''+macros[i]['name']+'\')" class="fas fa-trash-alt macro-i" style="right:5px;"></i></div>';
    }
    document.getElementById("macro-ul").innerHTML = rows;
}

function updateMacroList(){
    eel.read_config()(getMacroList);
}

function goToHome(){
    $('.dynamic-cnt').fadeOut(100);
    $('#homepage').fadeIn(200);
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
        document.getElementById('data').setAttribute("keycode", event.keyCode);
}


document.addEventListener('DOMContentLoaded', function(){ 

    eel.get_serial_ports()(listSerialPorts);
    eel.read_config()(getMacroList);
    getSerialPort();

    var colorWheel = new iro.ColorPicker("#colorWheelDemo", {
        width: 230,
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
        document.getElementById('color').style.backgroundColor = color.hexString;
        document.getElementById('color').innerText = color.hexString;
    });

    colorWheel.on('color:init', function(color){     
        document.getElementById('color').style.backgroundColor = color.hexString;
        document.getElementById('color').innerText = color.hexString;
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

    $(document).on('mouseover','.macro-li', function(){
        var buttons = $(this).find('.macro-i');
        console.log(buttons);
        $(buttons).each(function(i, btn) {
            $(btn).fadeIn(100);
        });
    });

    $(document).on('mouseout','.macro-li', function(){
        var buttons = $(this).find('.macro-i');
        console.log(buttons);
        $(buttons).each(function(i, btn) {
            $(btn).fadeOut(100);
        });
    });

    document.getElementById('new-action').addEventListener('click', function() {
        $('.dynamic-cnt').fadeOut(100);
        $('#new-macro-cnt').fadeIn(200);
    });

    document.getElementById('save').addEventListener('click', function() {
        $('.dynamic-cnt').fadeOut(100);
        $('#homepage').fadeIn(200);
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
