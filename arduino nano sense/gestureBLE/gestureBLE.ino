#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>

BLEService accService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create x characteristic and allow remote device to get notifications
BLEByteCharacteristic xCharacteristic("2A19", BLERead | BLENotify);


void setup() {
  //Serial.begin(9600);
  //while (!Serial);

 // begin initialization
  if (!BLE.begin()) {
    //Serial.println("starting BLE failed!");

    while (1);
  }

  pinMode(LED_BUILTIN, OUTPUT);
   if (!APDS.begin()) {
    //Serial.println("Failed to initialize IMU!");
    while (1);
  }


   // set the local name peripheral advertises
  BLE.setLocalName("gestureValues");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(accService);

  // add the characteristics to the service
  accService.addCharacteristic(xCharacteristic);
  

  // add the service
  BLE.addService(accService);

  xCharacteristic.writeValue(0);

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

  if (APDS.gestureAvailable()) {
    // a gesture was detected, read and print to serial monitor
    int gesture = APDS.readGesture();

    switch (gesture) {
      case GESTURE_UP:{
        char A = 'a';
        xCharacteristic.writeValue(A);
        break;
      }
      case GESTURE_DOWN:{
        char B = 'b';
        xCharacteristic.writeValue(B);
        break;
      }
      case GESTURE_LEFT:{
        char C = 'c';
        xCharacteristic.writeValue(C);
        break;
      }

      case GESTURE_RIGHT:{
        char D = 'd';
        xCharacteristic.writeValue(D);
        break;
      }

      default:
        // ignore
        break;
    }
  }
    }
}
digitalWrite(LED_BUILTIN, LOW);

}
