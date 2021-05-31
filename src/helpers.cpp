#include "helpers.h"

void spiffsInitialization() {
  Serial.print("Initializating SPIFFS...");
  if(!SPIFFS.begin(true)){
    Serial.println(" Error!");
    return;
  }
  Serial.println(" Done!");
}

void setResponseContent(httpsserver::HTTPResponse * res, String filePath) {
  File file = SPIFFS.open(filePath);
  
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  while(file.available()){
    String line = file.readStringUntil('\n');
    res->println(line);
  }
  
  file.close();
}

void wifiConnection(const char * ssid, const char * password) {
  Serial.print("Connecting to WiFi.");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Done!");
  Serial.print("Connected with IP = ");
  Serial.println(WiFi.localIP());
}

void dynamicDNSInitialization(const char * domain, const char * token) {
  Serial.print("Connecting to DuckDNS...");
  EasyDDNS.service("duckdns");
  EasyDDNS.client(domain, token);
  Serial.println(" Done!");
}

void dynamicDNSUpdate(int seconds) {
  Serial.print("Updating Dynamic DNS...");
  EasyDDNS.update(seconds * 1000);
  Serial.println(" Done!");
}
