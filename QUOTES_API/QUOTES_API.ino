    #include <WiFi.h>
    #include <HTTPClient.h>
    #include <ArduinoJson.h>


    //chnage pass below
    const char* ssid = "FELOMINO";
    const char* password = "SEAN_LOWE123";

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
      String payload = http.getString();
      Serial.println(payload);

    }
    http.end();
  }
  delay(60000);

