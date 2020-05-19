Final Project: Controlling PC using Arduino Nano BLE Sense
https://store.arduino.cc/usa/nano-33-ble-sense  (~$31)
Project Files link:  https://drive.google.com/drive/folders/1z3yzlgPYmeYHA0o347SrBd07SaUm5ZoA?usp=sharing
The Nano 33 BLE Sense (without headers) is Arduino’s 3.3V AI enabled board in the smallest available form factor: 45x18mm!
 The Arduino Nano 33 BLE Sense is a completely new board on a well-known form factor. It comes with a series of embedded sensors:
9-axis inertial sensor: what makes this board ideal for wearable devices
humidity, and temperature sensor: to get highly accurate measurements of the environmental conditions
barometric sensor: you could make a simple weather station
microphone: to capture and analyse sound in real time
gesture, proximity, light color and light intensity sensor: estimate the room’s luminosity, but also whether someone is moving close to the board
As we can see above, Arduino nano BLE sense is a hell of a deal offering at that price.
So, I planned on doing some project using this new board from Arduino. Since it also comes with an inbuilt Bluetooth Low Energy (BLE) module, it can be connected to various devices and transfer data.
To justify the title of this course (IoT), I intend to use cloud (Kafka) to transfer sensor data across the devices. First, the nano board is connected to a Raspberry Pi and sensor data is transferred via Bluetooth. Raspberry Pi uploads all the sensor data it received to the cloud. The PC we intend to control reads the data from the cloud and performs certain actions based on the data received.
The actions include:
play or pause of a video
minimizing or maximizing of current window
controlling cursor on the screen
opening an application
swapping between recently used applications, etc.
