#ifndef HELPERS_H
#define HELPERS_H

#include <SPIFFS.h>
#include <HTTPSServer.hpp>
#include <WiFi.h>
#include <EasyDDNS.h>

void spiffsInitialization();
void setResponseContent(httpsserver::HTTPResponse * res, String filePath);
void wifiConnection(const char * ssid, const char * password);
void dynamicDNSInitialization(const char * domain, const char * token);
void dynamicDNSUpdate(int seconds);

#endif
