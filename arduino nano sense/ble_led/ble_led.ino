#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>

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

  if (!APDS.begin()) {
    //Serial.println("Error initializing APDS9960 sensor!");
  }

   // set the local name peripheral advertises
  BLE.setLocalName("GestureLED");
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

    // while the central is still connected to peripheral:
    while (central.connected()) {

  if (APDS.gestureAvailable()) {
    // a gesture was detected, read and print to serial monitor
    int gesture = APDS.readGesture();

    switch (gesture) {
      case GESTURE_UP:{
        //Serial.println("Detected UP gesture");
        char g = 'o';
        ledCharacteristic.writeValue(g);
        gestureCharacteristic.writeValue(g);
      } break;

      case GESTURE_DOWN:{
        //Serial.println("Detected DOWN gesture");
        char g = 'f';
        ledCharacteristic.writeValue(g);
        gestureCharacteristic.writeValue(g);
      } break;

      case GESTURE_LEFT:{
        //Serial.println("Detected LEFT gesture");
        char g = 'o';
        ledCharacteristic.writeValue(g);
        gestureCharacteristic.writeValue(g);
      } break;

      case GESTURE_RIGHT:{
        //Serial.println("Detected RIGHT gesture");
        char g = 'f';
        ledCharacteristic.writeValue(g);
        gestureCharacteristic.writeValue(g);
      } break;

      default:
        // ignore
        break;
    }
  }
    }
}
}
