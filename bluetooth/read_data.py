from bluepy import btle
import RPi.GPIO as GPIO

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


p = btle.Peripheral('e9:78:ea:12:cf:86') # db:51:e9:77:c8:25 with line, e9:78:ea:12:cf:86
p.setDelegate(MyDelegate())

services=p.getServices()
# Setup to turn notifications on, e.g.
svc = p.getServiceByUUID("19B10010-E8F2-537E-4F6C-D104768A1214")
ch = svc.getCharacteristics()[1]
print(ch)
print(ch.valHandle)

p.writeCharacteristic(ch.valHandle+1, b"\x01\x00")

while True:
    if p.waitForNotifications(1.0):
        # handleNotification() was called
        continue
    # Perhaps do something else here
