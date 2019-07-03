#include <ESP8266WiFi.h>
const char* ssid     = "electrical_lab_els";
const char* password = "workhardplayhard";
const char* host = "103.52.146.51";
String data;
float temp;
float hum;
float power;
float roll;
float pitch;
float yaw;
void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  while (Serial.available()==0); 
        data = Serial.readString();    
        temp = getValue(data, ':', 1).toFloat();
        hum = getValue(data, ':', 2).toFloat();
        power = getValue(data, ':', 3).toFloat();
        
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "http://server.softwarecenter.web.id/sensor/6";
  url += "/"; url += temp; 
  url += "/"; url += hum;
  url += "/"; url += power;
  url += "/0"; //url += roll;
  url += "/0"; //url += pitch;
  url += "/0"; //url += yaw;


  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println();
  Serial.println("closing connection");
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";

}
