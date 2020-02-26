#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
//WIfi SSID & PASS
const char* ssid = "xxxxx";
const char* password = "xxxxx";

//HTTP Server URL
String url = "http://xxxxxxxxxx";

/********* Setup wifi ***************************
   setup wifi connect to wifi with the constants
   defined up
   while does not connect print a "."
   if connect then print the local ip
************************************************/

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print(F("Connecting to ")) ;
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);//begin the Serial comunnication
  setup_wifi();// connect to wifi
}
 
void loop()
{
   HTTPClient http;
 
   if (http.begin(url)) //Start connection
   {
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();  // GET Request
      if (httpCode > 0) {
         Serial.printf("[HTTP] GET... code: %d\n", httpCode);
 
         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();   // Get HTTP response
            Serial.println(payload);   // Print content of payload
         }
      }
      else {
         Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
 
      http.end();
   }
   else {
      Serial.printf("[HTTP} Unable to connect\n");
   }
 
   delay(15000);
}
