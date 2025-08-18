#include <WiFi.h>

const char* ssid = "Souham_Net";
const char* password = "pass@2307";

const int relayPin = 2;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/ON") != -1) {
      digitalWrite(relayPin, LOW);
    } else if (request.indexOf("/OFF") != -1) {
      digitalWrite(relayPin, HIGH);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println(R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ZenInfiny Smart Bulb</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    :root {
      --bulb-off-color: #e0e0e0;
      --bulb-on-color: #ffc107;
      --background-color: #2c3e50;
      --text-color: #ecf0f1;
      --glow-color: rgba(255, 193, 7, 0.7);
    }
    html, body {
      height: 100%;
      margin: 0;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      background-color: var(--background-color);
      font-family: 'Segoe UI', sans-serif;
      text-align: center;
      color: var(--text-color);
    }
    h1 {
      font-size: 2.5rem;
      margin-bottom: 10px;
    }
    .subtext {
      font-size: 1.2rem;
      margin-bottom: 40px;
    }
    .bulb-container {
      position: relative;
      width: 150px;
      height: 220px;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    #bulb::before {
      content: '';
      position: absolute;
      top: -45px;
      left: 50%;
      transform: translateX(-50%);
      width: 60px;
      height: 50px;
      background: linear-gradient(to right, #b0b0b0, #8c8c8c, #b0b0b0);
      border-radius: 5px 5px 0 0;
      border-bottom: 2px solid #555;
      z-index: 1;
    }
    #bulb {
      width: 150px;
      height: 150px;
      background: var(--bulb-off-color);
      border-radius: 50% 50% 50% 50% / 60% 60% 40% 40%;
      position: relative;
      transition: background-color 0.5s ease, box-shadow 0.5s ease;
    }
    #bulb.on {
      background-color: var(--bulb-on-color);
      box-shadow: 0 0 15px var(--glow-color), 0 0 30px var(--glow-color), 0 0 60px var(--glow-color);
    }
    #pull-chain {
      width: 4px;
      height: 100px;
      background-color: #bdc3c7;
      position: absolute;
      bottom: -80px;
      left: 50%;
      transform: translateX(-50%);
      cursor: pointer;
      transition: height 0.2s ease;
    }
    #pull-chain::after {
      content: '';
      position: absolute;
      bottom: -10px;
      left: 50%;
      transform: translateX(-50%);
      width: 12px;
      height: 12px;
      background-color: #bdc3c7;
      border-radius: 50%;
    }
    #pull-chain:active {
      height: 80px;
    }
    footer {
      position: absolute;
      bottom: 20px;
      font-size: 14px;
      color: #999;
    }
  </style>
</head>
<body>
  <h1>ZenInfiny Smart Bulb</h1>
  <div class="subtext">Pull the chain to control the light</div>
  <div class="bulb-container">
    <div id="bulb"></div>
    <div id="pull-chain"></div>
  </div>
  <footer>
    Developed by Sunanda Dutta &copy; 2025 ZenInfiny
    The Webpage Is Developed By Debanjan
  </footer>

  <script>
    const bulb = document.getElementById('bulb');
    const pullChain = document.getElementById('pull-chain');
    let isBulbOn = false;

    pullChain.addEventListener('click', () => {
      isBulbOn = !isBulbOn;

      if (isBulbOn) {
        bulb.classList.add('on');
        fetch('/ON').catch(error => console.error('Error:', error));
      } else {
        bulb.classList.remove('on');
        fetch('/OFF').catch(error => console.error('Error:', error));
      }
    });
  </script>
</body>
</html>
    )rawliteral");
    delay(1);
    client.stop();
  }
}
