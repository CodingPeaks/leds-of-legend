import eel
import yaml
import serial
import threading
import time
import keyboard
from serial import SerialException
from win32gui import GetWindowText, GetForegroundWindow, IsWindowVisible, EnumWindows
from win32process import GetWindowThreadProcessId
from psutil import Process
from ntpath import basename

config = {}
macros = '';
baudrate = 115200
kl = 0

ser = serial.Serial(timeout=1)

eel.init('web')

@eel.expose
def serial_begin(port):
    global ser
    if not port == "":
        if ser.isOpen():
            ser.close();
            print("Port successfully closed")
        ser = serial.Serial(port, baudrate, timeout=0.01)
        if ser.isOpen():
            print("Port successfully opened")
            set_serial_port(port)
            return 1
        else:
            print("Port open error!")
            return 0
    else:
    	if ser.isOpen():
            ser.close();
            if ser.isOpen():
                print("Port close error!")
                return 0
            else:
                print("Port successfully closed")
                return 1    

@eel.expose
def get_serial_ports():

    ports = ['COM%s' % (i + 1) for i in range(256)]
    
    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

@eel.expose
def serial_write(message):
    global ser
    if ser.isOpen():
        print("Sending message -> " + message)
        ser.write(message.encode('ascii')+'\r')
        return 1
    else:
        print("No serial port initialized")
        return 0

@eel.expose
def get_foreground():

    _, pid = GetWindowThreadProcessId(GetForegroundWindow())
    path = Process(pid).exe()
    proc_name = basename(path)
    return proc_name

def winEnumHandler( hwnd, procs ):
    if IsWindowVisible( hwnd ):
    	_, pid = GetWindowThreadProcessId(hwnd)
    	path = Process(pid).exe()
    	proc_name = basename(path)
    	if proc_name not in procs:
            procs.append(proc_name)

@eel.expose
def get_procs():
    procs = []
    EnumWindows(winEnumHandler, procs)
    return procs

@eel.expose
def read_config():
    global config, macros
    with open('config.yml') as f:
        docs = yaml.load_all(f, Loader=yaml.FullLoader)
        for doc in docs:
            for k, v in doc.items():
                #print(k, v)
                config.update({k : v})
    print(config)
    macros = config['Macros']
    return config

@eel.expose
def write_config(macros):
    with open('config.yml', 'w') as f:
        data = yaml.dump(macros, f)

@eel.expose
def add_macro(name, key, fadein, fadeout, brightness, color):
    new_macro = {}
    new_macro['name'] = name.encode("utf-8")
    new_macro['key'] = key.encode("utf-8")
    new_macro['fadein'] = fadein
    new_macro['fadeout'] = fadeout
    new_macro['brightness'] = brightness
    new_macro['color'] = color.encode("utf-8")
    config['Macros'].append(new_macro)
    with open('config.yml', 'w') as f:
        data = yaml.dump(config, f)
    return(config)

@eel.expose
def delete_macro(macro_name):
    name = macro_name.encode("utf-8")
    for macro in config['Macros']:
        if macro['name'] == name:
            index = config['Macros'].index(macro)
            config['Macros'].pop(index)
            print(config['Macros'])
    with open('config.yml', 'w') as f:
        data = yaml.dump(config, f)
    return(config)

@eel.expose
def set_serial_port(port):
    config['Serial']['Port'] = port.encode("utf-8")
    with open('config.yml', 'w') as f:
        data = yaml.dump(config, f)

@eel.expose
def get_serial_port():
    read_config()
    print(config['Serial']['Port'])
    return config['Serial']['Port']

@eel.expose
def start_kl():
    global kl
    kl = 1
    keylog()	

@eel.expose
def stop_kl():
    global kl
    kl = 0	

def keylog():
    global kl
    print keyboard.read_key()
    if kl == 1:
        threading.Timer(0.01, keylog).start()

eel.start('index.html', size=(1000, 600))

