#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "xxxxx";
const char* password = "xxxxx";

// WebServer Instantiation
ESP8266WebServer server(80);

//Web Page Variable
String webPage = "";
 
void setup(void){
  
  //Build Basic Web Page using HTML
  webPage += "<h1>AMAZING WEB SERVER</h1>";
  webPage += "<p><marquee>######.....IoT Rulz!!!.....######</marquee></p>";
  delay(1000);

  //Serial Comunnication and Wifi Connection kick off
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //Web Page handler on root "/" using .send() method from ESP8266WebServer class
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  //Start Web Server and notify via serial comm.
  server.begin();
  Serial.println("HTTP server started");

}
 
void loop(void){
  //Handle client requests to web Server using .handleClient() method from ESP8266WebServer class
  server.handleClient();
}
