#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "FELOMINO";
const char* password = "SEAN_LOWE123";

void setup(){
    Serial.begin(115200);
    delay(1000);

    WiFi.begin(ssid, password);
    Serial.println("\nConnecting....");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }

    Serial.println("\nConnected to the WiFi network!!!");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void loop(){
    if ((Wifi.status() == WL_CONNECTED)){
        
    }
    else {
        Serial.println("Disconnected :((")
    }
    delay(10000);
}
