import pyautogui
from kafka import KafkaConsumer
import struct

screenWidth, screenHeight = pyautogui.size()

currentMouseX, currentMouseY = pyautogui.position()

consumer = KafkaConsumer('test', bootstrap_servers=['iotproject.space:9092'])

def moveMouse(ordinate, value):
    if ordinate=='x':
        if value <0:
            pyautogui.move(0,10)
        else:
            pyautogui.move(0,-10)
    if ordinate=='y':
        if value <0:
            pyautogui.move(-10,0)
        else:
            pyautogui.move(10,0)

for message in consumer:
    print ("%s:%d:%d: key=%s value=%s" % (message.topic, message.partition,
                                          message.offset, message.key,
                                          struct.unpack('f',message.value)))
    moveMouse(message.key.decode('utf-8'), struct.unpack('f', message.value)[0])


    
    