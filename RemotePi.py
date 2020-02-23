from gpiozero import LED
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep

factory = PiGPIOFactory(host='192.168.1.197')
led = LED(9, pin_factory=factory)

while True:
    led.on()
    sleep(1)
    led.off()
    sleep(1)