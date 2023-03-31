#ifndef HELPERS_H
#define HELPERS_H

#include <SPIFFS.h>
#include <HTTPSServer.hpp>
#include <WiFi.h>
#include <EasyDDNS.h>

using namespace httpsserver;

void spiffsInitialization();
void setResponseContent(HTTPResponse * res, String filePath);
void wifiConnection(const char * ssid, const char * password);
void dynamicDNSInitialization(const char * domain, const char * token);
void dynamicDNSUpdate(int interval);
void dynamicDNSUpdateLoop(unsigned long * lastUpdateTime, int interval);

#endif
