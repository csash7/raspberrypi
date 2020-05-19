#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

float ax, ay, az;         // accelerometer values

//Defining Bluetooth Peripheral (this board)
//BLEPeripheral blePeripheral;  

//Defining HID Serivce
BLEService hidService("1812");    // declare it as an HID device
BLECharCharacteristic bootModeProtocol("2A42", BLERead | BLEWriteWithoutResponse);
BLECharacteristic bootMouseReport("2A33", BLERead | BLENotify,3);//Seting up the mouse specific characteristic
BLECharacteristic hidInformation("2A4A",BLERead, 4 );
BLECharacteristic reportMap("2A4B", BLERead,50);
BLECharCharacteristic controlPoint("2A4C",BLEWriteWithoutResponse);
BLECharacteristic mouseReport("2A4D", BLERead | BLENotify, 3); //the report value

//Defining Battery Service
BLEService batteryService("180F");
BLECharCharacteristic batteryLevel("2A19", BLERead);

//Device Information Service
BLEService deviceInformation("180A");
BLECharacteristic pnpID("2A50", BLERead, 7);

const int ledPin = 13;      // activity LED pin

int calibrateOffsets = 1; // int to determine whether calibration takes place or not

long prevmill;
int lastx;
int lasty;
unsigned char bleValue[3] = {0x00, 0x00, 0x00};
unsigned char reportMapValue[50] = {0x05,0x01,0x09,0x02,0xA1,0x01,0x09,0x01,0xA1,0x00,0x05,0x09,0x19,0x01,0x29,0x03,0x15,0x00,0x25,0x01,0x95,0x03,0x75,0x01,0x81,0x02,0x95,0x01,0x75,0x05,0x81,0x01,0x05,0x01,0x09,0x30,0x09,0x31,0x15,0x81,0x25,0x7F,0x75,0x08,0x95,0x02,0x81,0x06,0xC0,0xC0};
const unsigned char bootMode = 0x00;
const unsigned char batteryDummy = 0x63;
unsigned char hidInfo[3] = {0x4a, 0x00, 0xc0};
unsigned char pnp[4] = {0x02, 0x00, 0x00, 0x12};

float prevspeedx;
float prevspeedy;
float currentspeedx;
float currentspeedy;
byte movex;
byte movey;

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");
  IMU.begin();

  // verify connection
  Serial.println("Testing device connections...");
  if (IMU.begin()) {
    Serial.println("CurieIMU connection successful");
  } else {
    Serial.println("CurieIMU connection failed");
  }

  if (!BLE.begin()) {
    //Serial.println("starting BLE failed!");

    while (1);
  }


  // configure Arduino LED for activity indicator
  pinMode(LED_BUILTIN, OUTPUT);

  // Add service name and UUID for bluetooth
  BLE.setLocalName("accMouse");
  BLE.setAdvertisedServiceUuid(hidService.uuid());

  // Add services and characteristics
 
  //HID Service and Characteristics
  
  hidService.addCharacteristic(bootMouseReport);
  Serial.println("bootMouseReport added");
  hidService.addCharacteristic(bootModeProtocol);
  hidService.addCharacteristic(hidInformation);
  hidService.addCharacteristic(reportMap);
  hidService.addCharacteristic(controlPoint);
  hidService.addCharacteristic(mouseReport);
  BLE.addService(hidService);
  Serial.println("adding HID Service");

  //Battery service and characteristic
  batteryService.addCharacteristic(batteryLevel);
  BLE.addService(batteryService);

  //Device Information
  deviceInformation.addCharacteristic(pnpID);
  BLE.addService(deviceInformation);

  // Setting the initial value for our mouse input
  bootModeProtocol.writeValue('0');
  bootMouseReport.writeValue(bleValue,3);
  Serial.print("boot mouse report value:   ");
  Serial.print(bleValue[0]);
  Serial.print(bleValue[1]);
  Serial.print(bleValue[2]);
  batteryLevel.writeValue(batteryDummy);
  hidInformation.writeValue(hidInfo, 4);
  pnpID.writeValue(pnp, 7);
  reportMap.writeValue(reportMapValue,50);
  mouseReport.writeValue(bleValue,3);
  BLE.setAppearance(962);

  // Starting the service
  prevmill = millis();
  prevspeedx = 0;
  prevspeedy = 0;
  currentspeedx = 0;
  currentspeedy = 0;
  lastx = 0;
  lasty = 0;

  BLE.advertise();

}

void loop() {
  long currentmill = millis();
  int hertz = 200;
  if (currentmill - prevmill >= 1000 / hertz) {

    // look for central device to connect
    BLEDevice central = BLE.central();
    BLE.poll();

    // if a central is connected then we start to do things
    if (central) {

      while (central.connected()) {
      // read raw accel/gyro measurements from device
      if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(ax, ay, az);
      digitalWrite(LED_BUILTIN, HIGH);

      currentspeedx = prevspeedx + (54 * ax);
      currentspeedy = prevspeedy + (54 * ay);
      Serial.print(ay / 258);

      Serial.print("\t");
      Serial.println((az - 16384) / 258);

      movex = byte(floor(ay / 258));
      movey = byte(floor((az - 16384) / 258));

 
      bleValue[1] = movex;
      bleValue[2] = movey;
      bootMouseReport.writeValue(bleValue,3);
      prevspeedx = currentspeedx;
      prevspeedy = currentspeedy;
      prevmill = currentmill;
      }

    }

    digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
