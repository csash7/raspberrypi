#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

int accelX=1;
int accelY=1;
float x, y, z;

BLEService customService("1101");
BLEUnsignedIntCharacteristic customXChar("2101", BLERead | BLENotify);
BLEUnsignedIntCharacteristic customYChar("2102", BLERead | BLENotify);

void setup() {
IMU.begin();
Serial.begin(9600); 
while (!Serial);

pinMode(LED_BUILTIN, OUTPUT);

if (!BLE.begin()) {
Serial.println("BLE failed to Initiate");
delay(500);
while (1);
}

BLE.setLocalName("Arduino Accelerometer");
BLE.setAdvertisedService(customService);
customService.addCharacteristic(customXChar);
customService.addCharacteristic(customYChar);
BLE.addService(customService);
customXChar.writeValue(accelX);
customYChar.writeValue(accelY);

BLE.advertise();

Serial.println("Bluetooth device is now active, waiting for connections...");
}


void loop() {

BLEDevice central = BLE.central();
if (central) {
Serial.print("Connected to central: ");
Serial.println(central.address());
digitalWrite(LED_BUILTIN, HIGH);
while (central.connected()) {
delay(200);
read_Accel();

customXChar.writeValue(accelX);
customYChar.writeValue(accelY);

Serial.print("At Main Function");
Serial.println("");
Serial.print(accelX);
Serial.print(" - ");
Serial.println(accelY);
Serial.println("");
Serial.println("");
}
}
digitalWrite(LED_BUILTIN, LOW);
Serial.print("Disconnected from central: ");
Serial.println(central.address());
}

void read_Accel() {

if (IMU.accelerationAvailable()) {
IMU.readAcceleration(x, y, z);
accelX = (1+x)*100;
accelY = (1+y)*100;

}
}
