var last_key;
var last_event;

function bindKey(action) {
    if(action == 'keydown'){
        var cmd = '1;20;15';
    } else if(action == 'keyup') {
        var cmd = '0';
    }
 eel.serial_write('COM10', 115200, cmd)(callBack)
}

function callBack() {
 console.log('Key successfully binded!');
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