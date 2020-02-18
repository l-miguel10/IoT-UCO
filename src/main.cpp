#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "xxxxx";
const char* password = "xxxxx";

ESP8266WebServer server(80);

String webPage = "";
 
void setup(void){
  webPage += "<h1>AMAZING WEB SERVER</h1><p>LED INTERNO <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
  webPage += "<p>GPIO PORT<a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";
  webPage+= "<p><marquee>######.....IoT Rulz!!!.....######</marquee></p>";
  delay(1000);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); 
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
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    Serial.println("SOCKET 1 ON");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPage);
    Serial.println("SOCKET 1 OFF");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", webPage);
    Serial.println("SOCKET 2 ON");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", webPage);
    Serial.println("SOCKET 1 OFF");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
}
