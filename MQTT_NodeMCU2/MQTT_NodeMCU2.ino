#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <wifi.h> // Custom library to connect to the Wi-Fi
#include <sensors_actuators2.h> // Custom library to interact with the sensors and actuators of the NodeMCU2


const char* mqtt_server = "192.168.18.90"; // Address of the Raspberry Pi
const int onboard_led = 16;
int counter_button = 0;

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
   Serial.begin(115200);
  pinMode(onboard_led, OUTPUT);
  digitalWrite(onboard_led, HIGH);
  connect_to_wifi(); // Device connects to the Wi-Fi network
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  setup_sensors_and_actuators();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  if (!client.loop())
  {
    client.connect("NodeMCU2");
  }
}



void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("NodeMCU2"))
    {
      Serial.println( "connected");
      // Subscribe to receive the messages for the topics related with the sensors of the protoboard 1
      client.subscribe("button");
      client.subscribe("sonar");
      client.subscribe("servo");
      client.subscribe("send_local_ip_2");
      client.subscribe("send_mac_address_2");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Function executed when a message comes whose topic is subcribed this device
void callback(String topic, byte* message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  String messageTemp;
  for (int i=0; i<length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (topic == "button") {
    counter_button += 1;
    if (counter_button == 10) {
      counter_button = 0;
    }
    Serial.print("Counter: ");
    Serial.println(counter_button);
    show_number(counter_button);
  }
  else if (topic == "sonar") {
    float distance = get_distance_sonar();
    // Publish the measured value to the MQTT broker
    // As the Raspberry Pi is subscribed to these responses, the
    // values will be shown in the webpage of its server
    client.publish("value_distance", String(distance).c_str());
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  else if (topic == "servo") {
    int pos = messageTemp.toInt();
    move_servo(pos);
    Serial.print("Servo moved to position: ");
    Serial.println(pos);
  }
  else if (topic == "send_local_ip_2") {
    String local_ip = get_local_ip();
    client.publish("local_ip_2", local_ip.c_str());
    Serial.print("Local IP: ");
    Serial.println(local_ip);
  }
  else if (topic == "send_mac_address_2") {
    String mac_address = get_mac_address();
    client.publish("mac_address_2", mac_address.c_str());
    Serial.print("MAC address: ");
    Serial.println(mac_address);
  }
}
