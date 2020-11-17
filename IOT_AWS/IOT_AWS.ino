#include <FS.h>
#include <Secure_wifi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include<DHT.h>

#define DHTPIN 4  
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* AWS_endpoint = "a1c0tsr4vl83yp-ats.iot.ap-southeast-1.amazonaws.com"; //MQTT broker ip


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.printf("Message has arrived [%s]\n", topic);
}

//WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient); //set  MQTT port number to 8883 as per //standard
/*long lastMsg = 0;
char msg[50];
int value = 0;*/

void setup() {

  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  // initialize digital pin LED_BUILTIN as an output.
  dht.begin();
  setup_wifi();
  delay(1000);
  read_Certification();
  

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  temperature();
  delay(20000);
}

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Connecting to MQTT Broker....");
   
    if (client.connect("ESPthing")) {
      Serial.println("Connected");
      // Once connected, publish an announcement...
     
    } else {
      Serial.print("Cannot connect to MQTT Broker..");
      Serial.print(client.state());
      Serial.println("Reconnecting in 5 seconds .... ");
      //char buf[256];
      //espClient.getLastSSLError(buf,256);
      //Serial.print("Error SSL...");
      //Serial.println(buf);

      delay(20000);
    }
  }
}

void temperature(){
  //char payload[512];
   float t = dht.readTemperature();
  
   if (isnan(t)) { 
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }else{
        
      }
    
        StaticJsonBuffer <300> JSONbuffer;
        JsonObject& JSONencoder = JSONbuffer.createObject();
       
        JSONencoder["motorid"] = "A1";
        JsonObject& Temp = JSONencoder.createNestedObject("sensorData");
        Temp ["temperature"] = t;
        
      
        char JSONmessageBuffer[100];
        JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        client.publish("motors/A1/status",JSONmessageBuffer);
    
      
  
}
