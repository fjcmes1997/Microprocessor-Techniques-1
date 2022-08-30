#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <wifi.h> // Custom library to connect to the Wi-Fi
#include <sensors_actuators1.h> // Custom library to interact with the sensors and actuators of the NodeMCU1


const char* mqtt_server = "192.168.18.90"; // Address of the Raspberry Pi
const int onboard_led = 16;
const int button = D5;
int button_state = 0;

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);


void setup()
{
  Serial.begin(115200);
  pinMode(onboard_led, OUTPUT);
  pinMode(button, INPUT);
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
    client.connect("NodeMCU1");
  }
  // If button is pressed
  button_state = digitalRead(button);
  if (button_state == HIGH)
  {
    Serial.println("Button pressed");
    client.publish("button", "pressed"); // Send a signal when button is pressed
  }
  delay(100);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("NodeMCU1"))
    {
      Serial.println( "connected");
      // Subscribe to receive the messages for the topics related with the sensors of the protoboard 1
      client.subscribe("led");
      client.subscribe("ldr");
      client.subscribe("temperature");
      client.subscribe("send_local_ip_1");
      client.subscribe("send_mac_address_1");
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

  if (topic == "led") {
    if (messageTemp == "blue") {
      Serial.println("LED blue");
      set_color_rgb(0, 0, 255);
    } else if (messageTemp == "green") {
      Serial.println("LED green");
      set_color_rgb(0, 255, 0);
    } else if (messageTemp == "red") {
      Serial.println("LED red");
      set_color_rgb(255, 0, 0);
    } else if (messageTemp == "off") {
      Serial.println("LED off");
      set_color_rgb(0, 0, 0);
    }
  }
  else if (topic == "ldr") {
    int luminosity = get_luminosity();
    // Publish the measured value to the MQTT broker
    // As the Raspberry Pi is subscribed to these responses, the
    // values will be shown in the webpage of its server
    client.publish("value_luminosity", String(luminosity).c_str());
    Serial.print("Luminosity: ");
    Serial.print(luminosity);
    Serial.println(" lux");
  }
  else if (topic == "temperature") {
    float temperature = get_temperature();
    client.publish("value_temperature", String(temperature).c_str());
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("ºC");
  }
  else if (topic == "send_local_ip_1") {
    String local_ip = get_local_ip();
    client.publish("local_ip_1", local_ip.c_str());
    Serial.print("Local IP: ");
    Serial.println(local_ip);
  }
  else if (topic == "send_mac_address_1") {
    String mac_address = get_mac_address();
    client.publish("mac_address_1", mac_address.c_str());
    Serial.print("MAC address: ");
    Serial.println(mac_address);
  }
}
