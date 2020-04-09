var last_key;
var last_event;
var macros;
var colorWheel;

function startKeyLogger(){
    eel.start_kl()( function(){
        $('#start_kl').addClass('disabled-btn');
        $('#stop_kl').removeClass('disabled-btn');
        $('.animated-gradient').fadeIn(200);
    });
}

function stopKeyLogger(){
    eel.stop_kl()( function(){
        $('#stop_kl').addClass('disabled-btn');
        $('#start_kl').removeClass('disabled-btn');
        $('.animated-gradient').fadeOut(200);
    });
}

function testKeyBind(action) {
    if(action == 'keypress'){
        var fadein_delay = document.getElementById('fadein_delay').value;
        var brightness = document.getElementById('brightness').value;
        var rgb_color = colorWheel.color.rgb;
        var color = parseInt(rgb_color.r/100*brightness)+';'+ parseInt(rgb_color.g/100*brightness)+';'+ parseInt(rgb_color.b/100*brightness);
        var cmd = color+';'+fadein_delay;
    } else if(action == 'keyup') {
        var fadeout_delay = document.getElementById('fadeout_delay').value;
        var cmd = '0;0;0;'+fadeout_delay;
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
    options += '<option value="">Select serial port</option>';
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
    var mid = document.getElementById('macro_name').getAttribute('mid');
    var id = (mid) ? parseInt(mid) : Date.now();
    var name = document.getElementById('macro_name').value;
    var key = document.getElementById('data').getAttribute('key');
    var keycode = parseInt(document.getElementById('data').getAttribute('keycode'));
    var fadein = parseInt(document.getElementById('fadein_delay').value);
    var fadeout = parseInt(document.getElementById('fadeout_delay').value);
    var brightness = parseInt(document.getElementById('brightness').value);
    //var color = document.getElementById('color').innerText;
    var rgb_color = colorWheel.color.rgb;
    var color = parseInt(rgb_color.r/100*brightness)+';'+ parseInt(rgb_color.g/100*brightness)+';'+ parseInt(rgb_color.b/100*brightness);

    if(fadein && fadeout &&  brightness && key && name){

        if(macros) {
        	if(macros.some(item => item.keycode === keycode) == false){

	            if(macros.some(item => item.id === id) == false) {
	                //add macro se NON esiste l'id
	                eel.add_macro(id, name, key, keycode, fadein, fadeout, brightness, color)(updateMacroList);
	            } else {
	                //edit macro se esiste l'id
	                eel.edit_macro(id, name, key, keycode, fadein, fadeout, brightness, color)(updateMacroList);
	            }

            	$('#new-macro-cnt .form-control').val('');
            	document.getElementById('macro_name').removeAttribute('mid');
            } else {
                alert('Macro binded to this key already exists!')
            }

    	} else {
            eel.add_macro(id, name, key, keycode, fadein, fadeout, brightness, color)(updateMacroList);
        }   

	}else{
		alert("Please fill all forms before saving");
	}
}

function deleteMacro(macro_name){
    eel.delete_macro(macro_name)(updateMacroList);
}

function editMacro(macro){
    var id = $(macro).attr('mid');
    var name = $(macro).attr('mname');
    var key = $(macro).attr('mkey');
    var brightness = parseInt($(macro).attr('mbrightness'));
    var color = $(macro).attr('mcolor');
    var keycode = $(macro).attr('mkeycode');
    var fadein = $(macro).attr('mfadein');
    var fadeout = $(macro).attr('mfadeout');

    $('#macro_name').val(name);
    $('#macro_name').attr('mid', id);
    $('#data').attr('key', key);
    $('#data').val(key);
    $('#data').attr('keycode', keycode);
    $('#fadein_delay').val(fadein);
    $('#fadeout_delay').val(fadeout);
    $('#brightness').val(brightness);
    var rgb_arr = color.split(';');
    colorWheel.color.hexString = rgbToHex(rgb_arr[0], rgb_arr[1], rgb_arr[2]);
    colorWheel.color.value = brightness;

    $('.dynamic-cnt').fadeOut(100);
    $('#new-macro-cnt').fadeIn(200);

}

function getMacroList(list){
    macros = list['Macros'];
    var rows = '';
    for(i=0;i<macros.length;i++){
        rows += '<div class="macro-li" mid='+macros[i]['id']+' mname='+macros[i]['name']+' mkey='+macros[i]['key']+' mbrightness='+macros[i]['brightness']+' mcolor='+macros[i]['color']+' mkeycode='+macros[i]['keycode']+' mfadein='+macros[i]['fadein']+' mfadeout='+macros[i]['fadeout']+'>'+macros[i]['name']+'<i title="Delete" onclick="deleteMacro(\''+macros[i]['name']+'\')" class="fas fa-trash-alt macro-i" style="right:5px;"></i></div>';
    }
    document.getElementById("macro-ul").innerHTML = rows;
}

function updateMacroList(){
    eel.read_config()(getMacroList);
    goToHome();
}

function goToHome(){
    $('.dynamic-cnt').fadeOut(100);
    $('#homepage').fadeIn(200);
}

function componentToHex(c) {
  var hex = parseInt(c).toString(16);
  return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
  return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function splash(param) {
	var time = param;
 	setTimeout(function () {
   		$('#splashscreen').fadeOut(400);
 	}, time);
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
    let text =
        (event.shiftKey ? ' shiftKey' : '') +
        (event.ctrlKey ? ' ctrlKey' : '') +
        (event.altKey ? ' altKey' : '') +
        (event.metaKey ? ' metaKey' : '') +
        (event.repeat ? ' (repeat)' : ' ') + event.key.toUpperCase();
        //console.log(event);
        document.getElementById('data').value = text;
        document.getElementById('data').setAttribute("key", event.key);
        document.getElementById('data').setAttribute("keycode", event.keyCode);
}

function keyDownAnimation(key) {
	$('#data').css('color', 'transparent');
	$('.keypress-animation').html(key.toUpperCase());
	$('.keypress-animation').fadeIn(150);
	$('.keypress-animation').css('font-size', '7em');
}

function keyUpAnimation() {
	$('#data').css('color', 'white');
	$('.keypress-animation').css('font-size', '1em');
	$('.keypress-animation').fadeOut(150);
}

document.addEventListener('DOMContentLoaded', function(){ 

    eel.get_serial_ports()(listSerialPorts);
    eel.read_config()(getMacroList);
    getSerialPort();

    colorWheel = new iro.ColorPicker("#colorWheelDemo", {
        width: 230,
        color: "rgb(255, 0, 0)",
        display: 'grid',
        borderWidth: 1,
        borderColor: '#9572e8'
    });

    colorWheel.on('input:change', function(color, changes){
        document.getElementById('color').style.backgroundColor = color.hexString;
        document.getElementById('color').innerText = color.hexString;
        document.getElementById('brightness').value = parseInt(color.value);
        //var color = parseInt(color.rgb.r/100*color.value)+';'+ parseInt(color.rgb.g/100*color.value)+';'+ parseInt(color.rgb.b/100*color.value);
        //document.getElementById('color').setAttribute('rgb', color); 
    });

    colorWheel.on('color:init', function(color){     
        document.getElementById('color').style.backgroundColor = color.hexString;
        document.getElementById('color').innerText = color.hexString;
    });

    colorWheel.on('color:change', function(color){     
        document.getElementById('color').style.backgroundColor = color.hexString;
        document.getElementById('color').innerText = color.hexString;

    });

    document.getElementById('brightness').addEventListener('input', function() {
        console.log(this.value);
        colorWheel.color.value = this.value;
    });

    document.getElementById('serial-port').addEventListener('change', function() {
        eel.serial_begin(this.value);
    });

    document.getElementById('data').addEventListener('keypress', (event) => {
        event.preventDefault();
        if(event.key != last_key || event.type != last_event) {
            selectKey(event);
            testKeyBind(event.type);
            last_key = event.key;
            last_event = event.type;
            keyDownAnimation(event.key);
        }
    });

    document.getElementById('data').addEventListener('keyup', (event) => {
        if(event.type != last_event) {
            testKeyBind(event.type);
            last_event = event.type;
            keyUpAnimation();
        }
    });

    $(document).on('mouseover','.macro-li', function(){
        var buttons = $(this).find('.macro-i');
        $(buttons).each(function(i, btn) {
            $(btn).fadeIn(100);
        });
    });

    $(document).on('mouseout','.macro-li', function(){
        var buttons = $(this).find('.macro-i');
        $(buttons).each(function(i, btn) {
            $(btn).fadeOut(100);
        });
    });

    $(document).on('click','.macro-li', function(){
        editMacro(this);
    });

    document.getElementById('new-action').addEventListener('click', function() {
        $('#new-macro-cnt .form-control').val('');
        document.getElementById('macro_name').removeAttribute('mid');
        $('.dynamic-cnt').fadeOut(100);
        $('#new-macro-cnt').fadeIn(200);

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
