#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic gestureCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);


void setup() {
  //Serial.begin(9600);
  //while (!Serial);

 // begin initialization
  if (!BLE.begin()) {
    //Serial.println("starting BLE failed!");

    while (1);
  }

  pinMode(LED_BUILTIN, OUTPUT);
   if (!IMU.begin()) {
    //Serial.println("Failed to initialize IMU!");
    while (1);
  }


   // set the local name peripheral advertises
  BLE.setLocalName("AccelLED");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(gestureCharacteristic);

  // add the service
  BLE.addService(ledService);

  ledCharacteristic.writeValue(0);
  gestureCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  //Serial.println("Bluetooth device active, waiting for connections...");
  //Serial.println("Detecting gestures ...");

}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  BLE.poll();

  if (central) {
    //Serial.print("Connected to central: ");
    // print the central's MAC address:
    //Serial.println(central.address());
  digitalWrite(LED_BUILTIN, HIGH);
    // while the central is still connected to peripheral:
    while (central.connected()) {
      float x, y, z;

  if (IMU.accelerationAvailable()) {
    // a gesture was detected, read and print to serial monitor
    delay(500);
    IMU.readAcceleration(x, y, z);

      if(y < -0.3) {
        char g = 'f';
        ledCharacteristic.writeValue(g);
        gestureCharacteristic.writeValue(g);
      }
      else if(y > 0.3){
        char g = 'o';
        ledCharacteristic.writeValue(g);
        gestureCharacteristic.writeValue(g);
      }
       
  }
    }
}
}
