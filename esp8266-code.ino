/*DISCLAIMER: This code is provided "as is" without warranty of any kind, either express or implied, including but not limited to the implied warranties
of merchantability and fitness for a particular purpose. The author(s) of this code make no representation or warranties with respect to the accuracy or
completeness of the contents of this code and specifically disclaim any implied warranties of merchantability or fitness for any particular purpose.
The author(s) shall not be held liable for any direct, indirect, incidental, consequential, or other damages arising from the use of, or inability to
use, this code or any part thereof, including but not limited to any loss of data, lost profits, or business interruption, whether in an action of contract,
negligence, or other tortious action, even if the author(s) of this code have been advised of the possibility of such damages. Use of this code is entirely
at your own risk. This code is totaly free to use.*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <string.h>

// fill in the fields with your information
const char* ssid = "YOUR-WIFI-NAME";
const char* password = "YOUR-WIFI-PASSWORD";
const char* brokerUrl = "YOUR-BROKER-URL";    // ATTENTION: do NOT insert the 'amqp+ssl://' here. The port on the final part (:8883) of your Broker URL shouldn't be here either. 
const int brokerPort = 8883;                  // MQTT port is usually 8883
const char* mqttUsername = "YOUR-USERNAME";   // this is the user that you defined when creating the ActiveMQ broker.
const char* mqttPassword = "YOUR-PASSWORD";   // NOT your amazon AWS password. This is the password for the user created above
const char* mqttClientId = "YOUR-CLIENT-ID";  // client ID (anything that you can identify as your device later on the console)

//topicos
const char* topic1 = "messageInput";    // publisher
const char* topic2 = "messageOutput";   // subscriber
const char* topic3 = "fileInput";       // publisher
const char* topic4 = "fileOutput";      // subscriber
const char* topic5 = "functionInput";   // publisher
const char* topic6 = "functionOutput";  // subscriber

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("Your ESP8266 local IP: ");
  Serial.println(WiFi.localIP());

  espClient.setInsecure(); // SSL is off. Amazon MQ shoud be automatically assigned by AWS.

  mqttClient.setServer(brokerUrl, brokerPort);
  mqttClient.setCallback(callback);

  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqttClientId, mqttUsername, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
      // subscribing to the topics
      mqttClient.subscribe(topic2); 
      mqttClient.subscribe(topic4);
      mqttClient.subscribe(topic6);
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
   mqttClient.loop();
   
   // publish messages every 60 seconds.
   static unsigned long lastMillis = 0;
   unsigned long currentMillis = millis();
   if (currentMillis - lastMillis >= 60000) {
      lastMillis = currentMillis;
      mqttClient.publish(topic1, "Hello from ESP8266!");    // wait for a response
      mqttClient.publish(topic3, "Server, did you write this down?");   // Server should write the sentence on a .txt file and confrim the operation
      mqttClient.publish(topic5, "4 6");     // server should respond the sum of the numbers given.                           
   }
    
}

// receiving the answers and printing them.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message [");
  Serial.print(topic);
  Serial.print("]: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  Serial.println();
}
