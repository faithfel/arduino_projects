    #include <WiFi.h>
    #include <HTTPClient.h>
    #include <ArduinoJson.h>

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

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://zenquotes.io/api/random");
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String input = http.getString();

      StaticJsonDocument<512> doc;
      JsonDocument filter;
      filter["q"] = true;
      const char* q = doc["q"];


      DeserializationError error = deserializeJson(doc, input);

      serializeJsonPretty(doc, Serial);
      Serial.println(q);
    }
    http.end();
  }
  delay(30000);
}
