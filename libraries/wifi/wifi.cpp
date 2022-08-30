#include <ESP8266WiFi.h>


const char* ssid = "CAS MALAGA_2.4G";
const char* password = "120ductFIT";

void connect_to_wifi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - IP address: ");
  Serial.println(WiFi.localIP());
}

String get_local_ip()
{
  IPAddress raw_local_ip = WiFi.localIP();
  String local_ip = "";
  for (int i=0; i<4; i++) {
    local_ip += i ? "." + String(raw_local_ip[i]) : String(raw_local_ip[i]);
  }
  return local_ip;
}

String get_mac_address()
{
  String mac_address = WiFi.macAddress();
  return mac_address;
}