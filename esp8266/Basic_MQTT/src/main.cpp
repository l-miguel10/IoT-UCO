#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> 

const char* ssid = "Luis Miguel";
const char* password =  "1996miguel1027";
const char* mqttServer = "192.168.1.102";
const int mqttPort = 1883;
const char* TopicSub = "input";
const char* aliveTopic = "Alive";
const char* SRTopic = "statusRequest";
const char* outputTopic = "output";


String url = "http://worldtimeapi.org/api/timezone/";
WiFiClient espClient;
PubSubClient client(espClient);
HTTPClient http;
String url_time_zone = "";


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


/********* function get hour ***************************
In this function the http request is made to obtain 
the url of the server and request the time of the place 
that the user wants to consult
************************************************/
String obtener_hora(String url_time_zone){

  String payload_url = "";

  client.disconnect();

	if (http.begin(espClient, url_time_zone)) //start connection
	{
		Serial.print("[HTTP] GET...\n");
		int httpCode = http.GET();  // make request

		if (httpCode > 0) {
			Serial.printf("[HTTP] GET... code: %d\n", httpCode);

			if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
			  payload_url = http.getString();	// get answer
				Serial.println(payload_url);	// show response by serial
        client.publish(SRTopic,"Ok");
       
			}
		}
		else {
			Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      client.publish(SRTopic,"ERROR");  
    }


		http.end();
	}
	else {

		Serial.printf("[HTTP} Unable to connect\n");
	}

  
 
  return payload_url;
}


/********* MQTT Callback ***************************
   here is defined the logic to execute after
   a messages arrives in the desired
   topic, for this example the variable:
   TopicSub
************************************************/
void callback(char* topic, byte* payload, unsigned int length) {

  //Notify about message arrived 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
  String messagePayLoad = "";

  //Print the message received

  Serial.print("Message:");
 

  for (unsigned int i = 0; i < length; i++) {
  
    messagePayLoad += (char)payload[i];
  }
  url_time_zone += (url + messagePayLoad);
  //Prints with visualization purposes
  Serial.print(url_time_zone);
  Serial.println();
  Serial.println("-----------------------");
  String time = obtener_hora(url_time_zone);
 
}




void setup() {
 
  //Start Serial Communication
  Serial.begin(115200);
  
  //Connect to WiFi
  setup_wifi();

  //Connect to MQTT Broker
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  //MQTT Connection Validation
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client")) {
 
      Serial.println("connected");  
  client.publish(aliveTopic, "connect successful");
  client.subscribe(TopicSub);
    } else {
 
      Serial.println("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
   
  }
  
  //Publish to desired topic and subscribe for messages
  
 
  
}

//String obtener_json(String payload_url){
 // Allocate the JSON document
  //
  // Inside the brackets, 200 is the capacity of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  //StaticJsonDocument<260> doc;
  //char json[50];
  //payload_url.toCharArray(json,50);
  //client.publish(outputTopic, payload_url);
  //Serial.println("Output published");
  
  //DynamicJsonDocument doc(300);
  //deserializeJson(doc, json);
  
  //const char *date = doc["datetime"];
  // Test if parsing succeeds.
  //if (error) {
    //Serial.print(F("deserializeJson() failed: "));
    //Serial.println(error.c_str());
   // return;
  //}

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
 // const char* day = doc["day"];
 // const char* date = doc["date"];
 // long time = doc["time"];


//  String format_date = compose_date(year, month, day_of_month, day_of_week, hour);


  // Print values.
//  Serial.println(day);
//  Serial.println(time);
//  Serial.println(latitude, 6);
//  Serial.println(longitude, 6);
  
//  return payload_url;

// client.publish(outputTopic,payload_url);
//}



void loop() {
  //MQTT client loop
  //client.connect("ESP8266Client");
  client.loop();
  
}



 
