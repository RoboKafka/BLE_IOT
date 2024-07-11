#include "Nicla_System.h"
#include <ArduinoBLE.h>
#include <Arduino_BHY2.h>

// Define Service and Characteristic UUIDs (example values)
BLEService environmentService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLECharacteristic temperatureCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 4); // Temperature (float32)
BLECharacteristic accelerationCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 12); // Acceleration (3 * float32)

String name;

SensorXYZ accel(SENSOR_ID_ACC); 
Sensor temp(SENSOR_ID_TEMP);
bool isConnected = false;


void setup() {
  Serial.begin(115200);
  nicla::begin();
  nicla::leds.begin();
  nicla::leds.setColor(yellow);
  while (!Serial);

  // Initialize BHY2 and Sensors
  BHY2.begin();
  accel.begin();
  temp.begin();

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }
  String address = BLE.address();

  Serial.print("address = ");
  Serial.println(address);

  address.toUpperCase();

  name = "NiclaSenseME-";
  name += address[address.length() - 5];
  name += address[address.length() - 4];
  name += address[address.length() - 2];
  name += address[address.length() - 1];

  Serial.print("name = ");
  Serial.println(name);

  BLE.setLocalName(name.c_str());
  BLE.setDeviceName(name.c_str());
  BLE.setAdvertisedService(environmentService);

  environmentService.addCharacteristic(temperatureCharacteristic);
  environmentService.addCharacteristic(accelerationCharacteristic);
  if (BLE.addService(environmentService)) {
    Serial.println("Service added successfully");
  } else {
    Serial.println("Failed to add service");
  }
  
  BLE.setEventHandler(BLEConnected, onBLEConnected);
  BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);

  BLE.advertise();
  Serial.println("BLE Server started.");
}

void loop() {
  BLE.poll(); 
  if (!isConnected) {
      // Scanning logic
      BLEDevice pico = BLE.available();

      if (pico) {
        if (pico.localName() == "PicoWBLE") { // Replace with your Pico's local name
          // Connect to the Pico
          Serial.print("Found PicoWBLE - ");
          Serial.print(pico.address());
          Serial.print(" - ");
          Serial.println(pico.localName());

          if (pico.connect()) {
            Serial.println("Connected");
            isConnected = true; // Update connection status
            nicla::leds.setColor(red);
          } else {
            Serial.println("Failed to connect!");
          }
        }
      }
    }
  updateSensorData();
  delay(100); // Adjust sampling rate as needed
}


void onBLEConnected(BLEDevice central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    nicla::leds.setColor(red);
}


void onBLEDisconnected(BLEDevice central) {
    // Optional: could stop any ongoing processes that were started in onBLEConnected
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    BLE.advertise(); // Restart advertising
    isConnected = false;
    nicla::leds.setColor(off);
}

void updateSensorData() {
  BHY2.update(); 

  float temperature = temp.value();
  float acceleration[3] = {accel.x(), accel.y(), accel.z()};

  // Convert temperature to bytes
  byte temperatureBytes[4];
  memcpy(temperatureBytes, &temperature, sizeof(temperature));
  temperatureCharacteristic.writeValue(temperatureBytes, 4);

  // Convert acceleration to bytes
  byte accelerationBytes[12];
  memcpy(accelerationBytes, acceleration, sizeof(acceleration));
  accelerationCharacteristic.writeValue(accelerationBytes, 12);

  /* Print sensor data
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Acceleration: ");
  Serial.print(acceleration[0]);
  Serial.print(", ");
  Serial.print(acceleration[1]);
  Serial.print(", ");
  Serial.println(acceleration[2]);
  */
}

