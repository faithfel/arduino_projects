#include <WiFi.h>
#include <WebServer.h>


const int in1 = 26;
const int in2 = 25;
const int in3 = 33;
const int in4 = 32;


const char* ssid = "ESP32_CAR";
const char* password = "12345678";


WebServer server(80);


void setup() {
  Serial.begin(115200);


  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());


  server.on("/", handleRoot);
  server.on("/F", forward);
  server.on("/B", backward);
  server.on("/L", left);
  server.on("/R", right);
  server.on("/S", stopCar);

  server.begin();
}


void loop() {
  server.handleClient();
}


void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html><html>
    <head>
      <title>ESP32 Car Control</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body { text-align: center; font-family: sans-serif; }
        button {
          width: 100px; height: 50px;
          font-size: 16px; margin: 10px;
          background-color: #FABD2F;
          border-radius: 10px;
        }
        button:active    {
          background-color: #FB412C;
        }
        body {
          background-color: #1F1F1F;
        }
        h2 {
          color: #EBDBB2;
        }
      </style>
      <script>
        function sendCommand(cmd) {
          fetch("/" + cmd);
        }

        function setupButton(id, command) {
          const btn = document.getElementById(id);
          btn.addEventListener('mousedown', () => sendCommand(command));
          btn.addEventListener('mouseup', () => sendCommand('S'));
          btn.addEventListener('touchstart', () => sendCommand(command));
          btn.addEventListener('touchend', () => sendCommand('S'));
        }

        window.onload = () => {
          setupButton("forward", "F");
          setupButton("backward", "B");
          setupButton("left", "L");
          setupButton("right", "R");
        };
      </script>
    </head>
    <body>
      <h2>BSCS-2 ESP32 Smart RC Car</h2>
      <div>
        <button id="forward">Forward</button><br>
        <button id="left">Left</button>
        <button id="right">Right</button><br>
        <button id="backward">Backward</button>
      </div>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}


void forward() {
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);
  server.send(200, "text/plain", "Forward");
  Serial.print("forward \n");
}

void backward() {
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW);
  server.send(200, "text/plain", "Backward");
  Serial.print("backwards \n");
}

void left() {
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);
  server.send(200, "text/plain", "Left");
  Serial.print("left \n");
}

void right() {
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW);
  server.send(200, "text/plain", "Right");
  Serial.print("right \n");
}

void stopCar() {
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
  server.send(200, "text/plain", "Stop");

}