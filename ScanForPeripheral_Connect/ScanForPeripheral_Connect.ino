/*
Break down the services to multiple services, so we can plot only the ones needed. 
*/

#include <ArduinoBLE.h>


void setup() {
  Serial.begin(9600);
  while(!Serial);

  BLE.begin();

  Serial.println("BLE Central program started");

  

  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214"); // peripheral UUID

  // put your setup code here, to run once:

}

void loop() {

  BLEDevice peripheral = BLE.available();

  if (peripheral){
    // discovered a peripherl, print out address, local name and advertised service

    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "NiclaSenseME-2F1D") {
      return;
    }
  
    BLE.stopScan();
    
    controlled(peripheral);

    //peripherl disconnected, start scanning again.
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }

}

void controlled(BLEDevice peripheral){
  //connect to the peripheral
  Serial.println("Connecting...");

  if (peripheral.connect()){
    Serial.println("connected");
  } else{
    Serial.println("Failed to connect");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }


/* 
this is how your characteristics should be written in the Nicla side of the program. 
BLECharacteristic temperatureCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 4); // Temperature (Float32)
BLECharacteristic accelerationCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 12); // Acceleration (3 * Float32)
*/
  //retrieve the LED characterisitc
  BLECharacteristic temperatureCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic accelerationCharacteristic = peripheral.characteristic("19B10002-E8F2-537E-4F6C-D104768A1214");

  if (!temperatureCharacteristic){
    Serial.println("Peripheral does not have the characteristic");
    peripheral.disconnect();
    return;
  } else if (!temperatureCharacteristic.canRead()){
    Serial.println("Peripheral does not have a printable characteristic");
    peripheral.disconnect();
    return;
    // check if the acceleration characteristic is readable. 
  } else if (!accelerationCharacteristic.canRead()){
    Serial.println("Peripheral doesnot have a printable characteristic");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()){
    // while the peripheral is connected

    
    byte temperatureBytes[4];

    if (temperatureCharacteristic.readValue(temperatureBytes, 4)) {
      float temperature;
      memcpy(&temperature, temperatureBytes, sizeof(temperature));
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(",");
      Serial.println();
      
    } else {
      Serial.println("Failed to read temperature characteristic.");
    }
    
    
/*
    byte accelerationBytes[12];

    if (accelerationCharacteristic.readValue(accelerationBytes,12)){
      float acceleration[3];
      memcpy(acceleration, accelerationBytes, sizeof(acceleration));
      
      Serial.print(acceleration[0]);
      Serial.print(",");
      
      Serial.print(acceleration[1]);
      Serial.print(",");
      
      Serial.print(acceleration[2]);
      
      Serial.println();
      
      } else {
        Serial.println("Failed to read acceleration characteristic.");}
*/
    delay(10); // as set in the nicla sense. 

  }

}





