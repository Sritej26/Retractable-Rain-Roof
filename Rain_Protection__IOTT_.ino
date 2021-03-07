#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Servo.h>
#include<BlynkSimpleEsp8266.h>


 Servo servo;
//-------- Customise these values -----------
const char* ssid = "sri";
const char* password = "sritej26";

float sensorvalue2 = 0;

#define value A0
 #define ORG "r0fb71"//bv0eir
#define DEVICE_TYPE "nodemcu"//n
#define DEVICE_ID "t1"//nid
#define TOKEN "12345678"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char publishtopic[] = "iot-2/evt/dhtData/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
int rainstatus=0;
int s=0;
char auth[] = "pU4dhZdZf5KrYLYkbBTML_RFYjHufFyi";


 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);


void setup() {
  
  servo.attach(2);
  servo.write(0);
  delay(1000);
 Serial.begin(115200);
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
   Blynk.begin(auth , ssid, password);

}
 
void loop() {
  
  sensorvalue2 = analogRead(value);
    Blynk.run();

  if(sensorvalue2<=700)
  {
      if(rainstatus<1)
      { servo.write(180);
        s=1;
        delay(1000);
        rainstatus++;
        PublishData(sensorvalue2);
        }
  
      else{
           rainstatus++;
         if(rainstatus>=25)
           {
              rainstatus=0;
              
            }
        }
   }
   else{
    if(s==1)
    {
      servo.write(0);
      delay(1000);
      s=0;
      }
    PublishData(sensorvalue2);
    rainstatus=0;
    }

delay(1000);
}



void PublishData(float d){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"value\":";
  payload += d;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(publishtopic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}

BLYNK_WRITE(V0)
{ s=1;
  servo.write(0);
  delay(1000);
  }
  
BLYNK_WRITE(V1)
{ s=0;
  servo.write(180);
  delay(1000);
  }
