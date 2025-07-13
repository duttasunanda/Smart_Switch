#include <WiFi.h> // this is the wifi libary for esp32 board 
                        //if your board is esp8266 then change "WiFi.h" to "ESP8266WiFi.h"

const char* ssid = "smart bulb  "; // write here your wifi/hotspot name 
const char* password = "12345abcd"; // write here you wifi/hotspot password 

const int relayPin = 2;// relay singnal pin has been declared at GPIO 2 (chage accordingly)

WiFiServer server(80);//creates a web server object that listens incoming connections on port 80, default port for https traffic 

void setup() { // 'setup' is a stater code which starts the microcontroller and run the code only once 
  Serial.begin(115200);// it starts communicating between microcontroller and computer at the rate of 115200 bits per second data transfer/recive  
  pinMode(relayPin, OUTPUT);// sets relaypin(GPIO 2) as a output of the code 
  digitalWrite(relayPin, LOW);//sets relaypin(GPIO 2) off initially  

  WiFi.begin(ssid, password);// connects with your wifi 
  Serial.println("Connecting to WiFi...");// it prints the "Connecting to WiFi..." in the serial monitor
  while (WiFi.status() != WL_CONNECTED) {// condition statement (while) which will conform wifi connectivity
    delay(500);// gap of 0.5s
    Serial.print(".");// if connection fail print '.' in serial moniter
  }

  Serial.println("\nConnected to WiFi.");// when connection successfull print "\nConnected to WiFi." in serial
  Serial.print("IP Address: ");//  print ip address in serial 
  Serial.println(WiFi.localIP());// provide local ip and print in serial 

  server.begin(); //starts web server 
}

void loop() {// loop continue the instruction till the board gets power 
  WiFiClient client = server.available();// This line waits for a new client to connect to with web server (in browser)
  if (client) {// Checks if a valid client has connected
    String request = client.readStringUntil('\r');//Reads the incoming HTTP request (from the user's browser).
    client.flush();//Clears any remaining data in the buffer that we don't need.

    if (request.indexOf("/ON") != -1) {//condition statement if cilent requset 'ON'  
      digitalWrite(relayPin, HIGH);//relayPin will active means bulb will glow
    } else if (request.indexOf("/OFF") != -1) {//condition statement if cilent requset 'off'  
      digitalWrite(relayPin, LOW);//relayPin will deactive means bulb will turnoff
    }

    // Send decorative HTML
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(R"rawliteral(
<!DOCTYPE html>// html code for your web page 
<html>
<head>
  <title>ZenInfiny Smart Bulb</title>
  <style>
    body {
      background: #f5f7fa;
      font-family: 'Segoe UI', sans-serif;
      text-align: center;
      color: #333;
      padding: 50px;
    }
    h1 {
      color: #007BFF;
      font-size: 36px;
      margin-bottom: 10px;
    }
    .subtext {
      font-size: 18px;
      color: #555;
      margin-bottom: 30px;
    }
    .button {
      display: inline-block;
      padding: 15px 30px;
      margin: 20px;
      font-size: 20px;
      font-weight: bold;
      text-decoration: none;
      border-radius: 10px;
      transition: 0.3s;
    }
    .on {
      background-color: #28a745;
      color: white;
    }
    .on:hover {
      background-color: #218838;
    }
    .off {
      background-color: #dc3545;
      color: white;
    }
    .off:hover {
      background-color: #c82333;
    }
    footer {
      margin-top: 50px;
      font-size: 14px;
      color: #999;
    }
  </style>
</head>
<body>
  <h1>ZenInfiny Smart Bulb</h1>
  <div class="subtext">Control your lighting with ease</div>
  <a href="/ON" class="button on">Turn ON</a> 
  <a href="/OFF" class="button off">Turn OFF</a>
  <footer>
    Developed by Sunanda Dutta &copy; 2025 ZenInfiny
  </footer>
</body>
</html>
    )rawliteral");

    delay(1);
    client.stop();// data send to server and stop and restart the loop 
  }
}
