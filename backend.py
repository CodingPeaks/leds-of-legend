import eel
import serial
from win32gui import GetWindowText, GetForegroundWindow, IsWindowVisible, EnumWindows
from win32process import GetWindowThreadProcessId
from psutil import Process
from ntpath import basename

eel.init('web')

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
def serial_write(port, baudrate, message):
    print(message)
    ser = serial.Serial(port, baudrate, timeout=0.01)

    if ser.isOpen():
        ser.write(message.encode('ascii')+'\r\n')
        return "OK"

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

#print(get_serial_ports())
#print(serial_write("COM3", 921600, "RGBadfgdfgsdfgsdfgsdfgsdgsdfgsdfgsdfgsd"))
#print(get_foreground())
#print(get_procs())

eel.start('index.html', size=(1000, 600))