#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

BLEService accService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create x characteristic and allow remote device to get notifications
BLEByteCharacteristic xCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
// create y characteristic and allow remote device to get notifications
BLEByteCharacteristic yCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);


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
  BLE.setLocalName("accValues");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(accService);

  BLEDescriptor xDescriptor("19B10013-E8F2-537E-4F6C-D104768A1214", "xValues");
  xCharacteristic.addDescriptor(xDescriptor);

  // add the characteristics to the service
  accService.addCharacteristic(xCharacteristic);
  accService.addCharacteristic(yCharacteristic);
  

  // add the service
  BLE.addService(accService);

  xCharacteristic.writeValue(0);
  yCharacteristic.writeValue(0);

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
    delay(500);
    // a gesture was detected, read and print to serial monitor
    IMU.readAcceleration(x, y, z);

      if (x>=0.3){
        char a = 'a';
      xCharacteristic.writeValue(a);
      }
      else if (x <= -0.3){
        char a = 'b';
        xCharacteristic.writeValue(a);
       }
      if(y>=0.3){
        char a = 'c';
      yCharacteristic.writeValue(a);
      }
      else if (y<=-0.3){
        char a = 'd';
        yCharacteristic.writeValue(a);
        }
       
  }
    }
}
digitalWrite(LED_BUILTIN, LOW);

}
