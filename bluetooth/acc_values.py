from bluepy import btle
import RPi.GPIO as GPIO
import time
from bluepy.btle import Scanner, DefaultDelegate
from kafka import KafkaProducer
import sys
import struct

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(21, GPIO.OUT, initial=GPIO.LOW)

producer = KafkaProducer(bootstrap_servers=['iotproject.space:9092'], api_version=(2,4,0))


class MyDelegate(btle.DefaultDelegate):
    def __init__(self):
        btle.DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        #state = int.from_bytes(data, byteorder='little')
        #state = data.decode('utf-8')
        print("A notification was received: %s" %data)
        if cHandle == 12:
            producer.send('test',key=b'x',value=data).get(timeout=30)
        else:    
            producer.send('test',key=b'y',value=data).get(timeout=30)


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
        print ('waiting for Data from the device.')
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
                print ('Found known device: '+ str(dev.addr))
                connected = True
                return dev.addr
        print ('No known device found.')
        
    #check.notify()
    #check.release()

if __name__ == '__main__':
    addr = ''
    known_devices = ['e9:78:ea:12:cf:86', 'db:51:e9:77:c8:25']
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
                x = svc.getCharacteristics()[0]
                y = svc.getCharacteristics()[1]
                #ch.write(b"\x01\x00")
                p.writeCharacteristic(x.valHandle+1, b"\x01\x00")
                p.writeCharacteristic(y.valHandle+1, b"\x01\x00")
                get_data(p)
            except:
                p.disconnect()
                print ('Disconnected from the device: '+ str(addr))
                print ("Unexpected error:", sys.exc_info()[0])
                addr = ''
