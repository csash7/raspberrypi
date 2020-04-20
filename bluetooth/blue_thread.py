from bluepy import btle
import RPi.GPIO as GPIO
import time
from bluepy.btle import Scanner, DefaultDelegate

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(21, GPIO.OUT, initial=GPIO.LOW)

class MyDelegate(btle.DefaultDelegate):
    def __init__(self):
        btle.DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        state = data.decode('utf-8')
        print("A notification was received: %s" %state)
        if state=='o':
            GPIO.output(21, GPIO.HIGH)
        elif state=='f':
            GPIO.output(21, GPIO.LOW)

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            #print ("Discovered device", dev.addr)
           """
        elif isNewData:
            print ("Received new data from", dev.addr)
           """

#check = threading.Condition()

scanner = Scanner().withDelegate(ScanDelegate())

def get_data(p):
    #check.acquire()
    #check.wait()
        while True:
            if p.waitForNotifications(1.0):
        # handleNotification() was called
                continue
    

def scan(known_devices):
    connected = False
    while not connected:
        print ('Scanning for devices.....')
        devices = scanner.scan(3.0)
        for dev in devices:
            if dev.addr in known_devices:
                print ('Found device: '+ str(dev.addr))
                connected = True
                return dev.addr
        print ('No known device found.')
        
    #check.notify()
    #check.release()

if __name__ == '__main__':
    addr = ''
    known_devices = ['e9:78:ea:12:cf:86', 'db:51:e9:77:c8:25 ']
   # Thread(target = func1, args = (connected, known_devices)).start()
    #Thread(target = func2, args = (connected, known_devices)).start()
    while True:
            addr = scan(known_devices)
            try:
                p = btle.Peripheral(addr)
                print ('Connected to device: '+ str(addr))
                p.setDelegate(MyDelegate())
                services=p.getServices()
                svc = p.getServiceByUUID("19B10010-E8F2-537E-4F6C-D104768A1214")
                ch = svc.getCharacteristics()[1]

                p.writeCharacteristic(ch.valHandle+1, b"\x01\x00")
                get_data(p)
            except:
                p.disconnect()
                print ('Disconnected from the device: '+ str(addr))
                addr = ''