#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Built-in RGB LED for most S3 DevKits is GPIO 48
#define RGB_PIN 48

// Nordic UART UUIDs (Required by Bluefruit Connect)
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println(">>> Device Connected");
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println(">>> Device Disconnected. Advertising...");
      pServer->getAdvertising()->start();
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String value = pCharacteristic->getValue();
      uint8_t* data = (uint8_t*)value.c_str();
      size_t len = value.length();

      if (len > 0) {
        // Check if this is a "Controller" packet
        if (data[0] == '!') {
          if (len >= 5 && data[1] == 'C') {
            // It's a Color Picker packet: !C [R] [G] [B] [CRC]
            uint8_t r = data[2];
            uint8_t g = data[3];
            uint8_t b = data[4];
            rgbLedWrite(RGB_PIN, r, g, b);
            Serial.printf("[RGB Update] R:%d G:%d B:%d\n", r, g, b);
          } 
          else if (data[1] == 'B') {
            // It's a Button packet: !B [Button Number] [1 for press / 0 for release] [CRC]
            Serial.printf("[Button] ID: %c State: %c\n", data[2], data[3]);
          }
        } 
        else {
          // It's just a normal UART text message
          Serial.print("Message from Phone: ");
          Serial.println(value);
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  
  // S3-Specific: Wait for Serial Monitor to catch up
  unsigned long start = millis();
  while (!Serial && millis() - start < 5000);

  // Initialize RGB LED
  rgbLedWrite(RGB_PIN, 0, 0, 0);

  BLEDevice::init("S3-UART-RGB-Combo");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  // TX for sending data TO phone
  pTxCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_TX,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  pTxCharacteristic->addDescriptor(new BLE2902());

  // RX for receiving data FROM phone
  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
                                           CHARACTERISTIC_UUID_RX,
                                           BLECharacteristic::PROPERTY_WRITE
                                         );
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("System Online. You can use UART or Controller in Bluefruit App.");
}

void loop() {
  // Bidirectional UART: Computer -> Phone
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    if (deviceConnected) {
      pTxCharacteristic->setValue(msg.c_str());
      pTxCharacteristic->notify();
      Serial.println("Sent to phone: " + msg);
    }
  }
  delay(10);
}