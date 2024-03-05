#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESPmDNS.h>

#define SSID "CST WIFI"
#define PSSWD "iotdevs123"

#define Dht_Sensor_Pin 9
#define Dht_type DHT11
float Humi = 0.0;
float Temp = 0.0;



DHT dht_sensor(Dht_Sensor_Pin, Dht_type);
WebServer server(80);

void handle_on_sensor();
void handle_on_notfound();
void handle_on_connect();

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  char temp[400];

  Humi = dht_sensor.readHumidity();
  Temp = dht_sensor.readTemperature();

  snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Temperature and Humidity from ESP32!</h1>\
    <p>Humidity: %02f</p>\
    <p>Temperature: %02f</p>\
  </body>\
</html>",

           Humi, Temp
          );
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PSSWD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() {
  server.send(200, "text/plain", "this works as well");

  });
  
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {

  server.handleClient();
                                                                  // read the analog / millivolts value for pin 2:
  Humi = dht_sensor.readHumidity();
  Temp = dht_sensor.readTemperature();
  
                                                                  // print out the values you read:
  Serial.print(Humi);
  Serial.println();
  Serial.print(Temp);
  Serial.println();
  
  delay(1000);                                                     // delay in between reads for as dht11 has 1khz frequency 
}
