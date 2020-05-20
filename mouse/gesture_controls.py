import pyautogui
from kafka import KafkaConsumer
import subprocess

screenWidth, screenHeight = pyautogui.size()

currentMouseX, currentMouseY = pyautogui.position()

consumer = KafkaConsumer('test', bootstrap_servers=['34.73.33.253:9092'])

def moveMouse(ordinate, value):
    if ordinate=='x':
        if value == 'a':
            subprocess.call(['C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe'])
        elif value == 'c':
            pyautogui.hotkey('alt', 'tab')
        elif value == 'b':
            pyautogui.hotkey('win', 'm')
        else:
            pyautogui.press('playpause')


for message in consumer:
    print ("%s:%d:%d: key=%s value=%s" % (message.topic, message.partition,
                                          message.offset, message.key,
                                          message.value.decode('utf-8')))
    moveMouse(message.key.decode('utf-8'), message.value.decode('utf-8'))


    
    