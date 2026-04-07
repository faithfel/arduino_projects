#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


//chnage pass below
const char* ssid = "ssid";
const char* password = "pass";

void setup(){
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
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
    if ((WiFi.status() == WL_CONNECTED)){
        long rnd = random(1, 50);
        HTTPClient client;

        client.begin("https://zenquotes.io/api/quotes" + String (rnd));
        int httpCode = client.GET();

        if (httpCode > 0) {
            String payload = client.getString();
            Serial.println("\n Statuscode" + String(httpCode));
            Serial.println(payload);

        }
        else {
            Serial.println("Error on HTTP");
        }
    }
    else {
        Serial.println("Disconnected :((");
    }
    delay(10000);
}
