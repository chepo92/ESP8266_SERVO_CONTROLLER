/*
  Project made and maintained by Kumar Aditya

  ESP8266 Servo Controller

  Create a ESP8266 Webserver for controlling the real-time position of the servo motor attached to ESP8266.
  
  
*/

// Import libraries 
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>


const char *ssid = "REPLACE_WITH_YOUR_SSID";         // replace with your SSID
const char *password = "REPLACE_WITH_YOUR_PASSWORD"; // replace with your Password
// Base, Shoulder, Elbow, Grip
const uint8_t servoPin1 = D5;                         // replace with servo pin
const uint8_t servoPin2 = D6;                         // replace with servo pin
const uint8_t servoPin3 = D7;                         // replace with servo pin
const uint8_t servoPin4 = D8;                         // replace with servo pin


/* Create Servo Object */
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;


// Create Server instance
AsyncWebServer server(80);
void setup()
{
  // Attach Servo, start SPIFFS and Connect to WiFi
  Serial.begin(115200);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);


  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\nConnected to the WiFi network: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  // Send home page from SPIFFS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Send index.html.");
    request->send(SPIFFS, "/index.html", "text/html");
  });
  // Receive Angle from client and process it 
  server.on("/angle", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    String servoIndexStr = request->arg("index");
    int servoIndex  = servoIndexStr.toInt();
    switch (servoIndex)
    {
    case 1:
      Serial.println("Current Position: " + angle + "°");
      servo1.write(angle.toInt());
      break;
    case 2:
      Serial.println("Current Position: " + angle + "°");
      servo2.write(angle.toInt());
      break;
    case 3:
      Serial.println("Current Position: " + angle + "°");
      servo3.write(angle.toInt());
      break;
    case 4:
      Serial.println("Current Position: " + angle + "°");
      servo4.write(angle.toInt());
      break;                
    default:
      Serial.println("Unknown servo" + servoIndexStr);
      break;
    }


    request->send(200);
  });

  // Send Favicon 
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  // Begin Server
  server.begin();
}
void loop()
{
}
