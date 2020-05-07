#include<Wire.h>
#include<SPI.h>
#include <UIPEthernet.h>
#include "PubSubClient.h"

#define CLIENT_ID       "TH00050"
#define INTERVAL        5000 // 3 sec delay between publishing


uint8_t mac[]   = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };      //mac for ethernet shield

#define STATIC 1 // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
IPAddress ip(192, 168, 86, 200);                     //change it according  to the  IP Of the network
// gateway ip address
IPAddress server(192, 168, 86, 1);
#endif

EthernetClient ethClient; 
PubSubClient client(ethClient);    
float lastTemp = -100;
float lastHumidity = -100;
String sensorId = "THK000001"; // Sensor ID. IMPORTANT - CHANGE IT FOR EVERY DEVICE.
char msgBuffer[20];
long previousMillis;
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived");
  Serial.print(topic);
  Serial.print("_");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    // Attempt to connect
    if (client.connect(CLIENT_ID, "ubuntu", "Ags@2019")) {
      Serial.println(F("connected"));
    } else {
     Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
 Wire.begin(); // join i2c bus (address optional for master)
Serial.begin(9600); // start serial for output
Ethernet.begin(mac,ip);
client.setServer("mqtt.croplytics.com", 1883);
client.setCallback(callback);
  delay(10);
previousMillis=millis();
}

void loop() {
  Wire.requestFrom(2, 15); // request 20 bytes from slave device #2
  String sensor,Humidity,Temperature;
  if (millis() - previousMillis > INTERVAL) {
    do                                 // slave may send less than requested
{
char c = Wire.read(); // receive a byte as character
sensor = sensor + c; //Keep saving whatever is comming
} while (Wire.available());

Temperature = Temperature.substring(0, 5); //slpit String from 0 to 5
Humidity = Humidity.substring(6); // Split from 6 to the End of String
float  temperature = Temperature.toFloat();
float  humidity = Humidity.toFloat();
 Serial.println("Temeprature");
 Serial.println(Temperature);
    Serial.println("");
  Serial.println("HUMIDITY");
   Serial.println(Humidity);
   if (temperature!= lastTemp || humidity!= lastHumidity)
    {
      lastTemp = temperature;
      lastHumidity = humidity;
      sendData(temperature, humidity);
    }
    previousMillis = millis();
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
//Sending data over MQTT
void sendData(float temperature, float humidity) {
  //Sending temperature and humidity
  String sTemperature = String(temperature, 2);
  String sHumidity = String(humidity, 2);
  String data_to_send = sensorId + "_" + sTemperature + "_" + sHumidity;
  Serial.println(F("Publishing New Data"));
  Serial.println(data_to_send );
  client.publish(getTopic("Sensor/").c_str(), data_to_send.c_str());
  delay(5000);
}

String getTopic(String topic)
{
  return topic + sensorId;
}

 
  
  

     
