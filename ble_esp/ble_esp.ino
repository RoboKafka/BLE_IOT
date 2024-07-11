#include <ArduinoBLE.h> // Use ArduinoBLE library

// UUIDs defined in the Nicla Sense ME sketch
#define BLE_SENSE_UUID(val) ("19b10000-" val "-537e-4f6c-d104768a1214")
const char* serviceUUID = BLE_SENSE_UUID("0000");
const char* temperatureUUID = BLE_SENSE_UUID("2001");
// ... (Add other UUIDs as needed)

BLEService niclaService(serviceUUID); // Create BLE Service

// Create BLE Characteristics with appropriate data types
BLEFloatCharacteristic temperatureCharacteristic(temperatureUUID, BLERead | BLENotify); // Assuming temperature is a float
// ... (Create other characteristics for humidity, pressure, etc.)


void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE Server!");

    BLE.init();
    BLE.setLocalName("ESP32_Server"); // Give your ESP32 a name
    BLE.setAdvertisedService(niclaService); 

    // Add characteristics to the service
    niclaService.addCharacteristic(temperatureCharacteristic);
    // ... (Add other characteristics to the service)

    BLE.addService(niclaService); // Add the service

    // Start advertising
    BLE.advertise();
    Serial.println("Waiting for Nicla Sense ME to connect...");
}

void loop() {
    BLEDevice central = BLE.central();  // Check for connections

    if (central) {
        Serial.println("Nicla Sense ME connected");

        while (central.connected()) {
            // Read data from the characteristics
            if (temperatureCharacteristic.written()) { // Check if value was updated
                float temperatureValue = temperatureCharacteristic.value();
                Serial.print("Temperature Received: ");
                Serial.println(temperatureValue); 

                // ... (Process the received data as needed)
            }
            // ... (Repeat for other characteristics)
        }
        
        Serial.println("Nicla Sense ME disconnected");
    }
}
