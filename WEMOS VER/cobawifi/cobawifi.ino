// Switch built-in LED of ESP32 via web server.  Based on
// https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino

#include "WiFi.h"

const char* ssid = "webserver test";
const char* password = "11111111";
const int ledPin = LED_BUILTIN;


const char* html = R""""(
HTTP/1.1 200 OK
Content-type:text/html

<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Controller</title>
  <style>
    body { text-align: center; font-family: Arial, sans-serif; }    
    button { background-color: slateblue; color: white; border: none; width: 50px; height: 30px; }
    button:hover { background-color: darkslateblue; }    
    button:active { background-color: mediumslateblue; }    
  </style>  
</head>
<body>
  <h1>ESP32 Controller</h1>
  <p>LED:
    <a href="/led/on"><button>ON</button></a>
    <a href="/led/off"><button>OFF</button></a>
  </p>
</body>
</html>
)"""";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.accept();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println(html);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Execute commands
        if (currentLine.endsWith("GET /led/on")) {
          digitalWrite(ledPin, LOW);
        }
        if (currentLine.endsWith("GET /led/off")) {
          digitalWrite(ledPin, HIGH);
        }
      }
    }
    client.stop();
  }
}