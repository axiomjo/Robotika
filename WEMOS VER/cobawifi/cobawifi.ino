#include "WiFi.h"

const char* ssid = "Qbar";
const char* password = "3Dward2@22";

const char* ssid2 = "AP TEST";
const char* password2 = "";

const int ledPin = LED_BUILTIN;

String header;
bool motorRunning = 0;

const char* html = R""""(
HTTP/1.1 200 OK
Content-type:text/html

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BOCCHI RC</title>
    <style>
        body {
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            justify-content: flex-start;
            align-items: center;
            height: 100vh;
        }

        .center-line {
            position: absolute;
            background-color: #000;
        }

        .vertical-line {
            left: 50%;
            width: 2px;
            height: 100%;
            transform: translateX(-50%);
        }

        .horizontal-line {
            top: 50%;
            height: 2px;
            width: 100%;
            transform: translateY(-50%);
        }

        .diamond-container {
            display: flex;
            justify-content: center;
            align-items: center;
            width: 100vw; 
            height: 100vh; 
        }

        .diamond-container-inner {
            position: relative;
            width: 200px;
            height: 200px;
        }

        .diamond-button {
            position: absolute;
            width: 150px;
            height: 50px;
            background-color: #ff36d7;
            border: none;
            color: white;
            font-size: 16px;
        }

        .diamond-button:active {
            background-color: #fc90e7;
        }

        .diamond-button-top {
            top: -50px;
            left: 25px;
        }

        .diamond-button-bottom {
            bottom: 50px;
            left: 25px;
        }

        .diamond-button-left {
            top: 25px;
            left: -60px;
        }

        .diamond-button-right {
            top: 25px;
            right: -60px;
        }

        .title-main {
            text-align: center;
            font-family: Arial, sans-serif;
            margin-top: 20px; 
            color: #000000;
            margin-top: 20px; 
            text-shadow: 
                -2px -2px 0 #fff,  
                2px -2px 0 #fff,
                -2px 2px 0 #fff,
                2px 2px 0 #fff;
        }
    </style>
</head>

<body>
    <div class="title-main">
        <h1>Bocchi Remote Control Car</h1>
    </div>
    <div class="diamond-container">
        <div class="diamond-container-inner">
            <button class="diamond-button diamond-button-top" onmousedown="turnOn(1)" onmouseup="turnOff(1)" ontouchstart="turnOn(1)" ontouchend="turnOff(1)">Maju</button>
            <button class="diamond-button diamond-button-bottom" onmousedown="turnOn(2)" onmouseup="turnOff(2)" ontouchstart="turnOn(2)" ontouchend="turnOff(2)">Mundur</button>
            <button class="diamond-button diamond-button-left" onmousedown="turnOn(3)" onmouseup="turnOff(3)" ontouchstart="turnOn(3)" ontouchend="turnOff(3)">Kiri</button>
            <button class="diamond-button diamond-button-right" onmousedown="turnOn(4)" onmouseup="turnOff(4)" ontouchstart="turnOn(4)" ontouchend="turnOff(4)">Kanan</button>
        </div>
    </div>

    <script>
        function turnOn(buttonId) {
            (buttonId == 1) ? forward() : (buttonId == 2) ? reverse() : (buttonId == 3) ? left() : right();
        }

        function turnOff(buttonId) {
            motorOff();
        }

        function forward(){
            fetch("/motor/forward");
        }

        function reverse(){
            fetch("/motor/reverse");
        }

        function left(){
            fetch("/motor/left");
        }

        function right(){
            fetch("/motor/right");
        }

        function motorOff(){
            fetch("/motor/off")
        }
    </script>
</body>

</html>
)"""";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid2);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // // Connect to Wi-Fi network with SSID and password
  // Serial.print("Connecting to ");
  // Serial.println(ssid);
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // // Print local IP address and start web server
  // Serial.println("");
  // Serial.println("WiFi connected.");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

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


        if (currentLine.endsWith("GET /motor/forward")&& motorRunning == 0) {
          digitalWrite(ledPin, 1);
          motorRunning = 1;
          delay(10);
        }
        if (currentLine.endsWith("GET /motor/reverse")&& motorRunning == 0) {
          digitalWrite(ledPin, 1);
          motorRunning = 1;
          delay(10);

        }
        if (currentLine.endsWith("GET /motor/left")&& motorRunning == 0) {
          digitalWrite(ledPin, 1);
          motorRunning = 1;
          delay(10);

        }
        if (currentLine.endsWith("GET /motor/right") && motorRunning == 0) {
          digitalWrite(ledPin, 1);
          motorRunning = 1;
          delay(10);
          
        }
        if (currentLine.endsWith("GET /motor/off") && motorRunning == 1) {
          digitalWrite(ledPin, 0);
          motorRunning = 0;
          delay(10);
        }
      }
    }
    client.stop();
  }
}
