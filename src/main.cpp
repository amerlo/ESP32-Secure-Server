#include <Arduino.h>

#include <SSLCert.hpp>
#include <HTTPSServer.hpp>

#include "ssl/cert.h"
#include "ssl/private_key.h"

#include "constants.h"
#include "helpers.h"
#include "serverHandlers.h"

using namespace httpsserver;

SSLCert cert = SSLCert(example_crt_DER, example_crt_DER_len, example_key_DER, example_key_DER_len);
HTTPSServer secureServer = HTTPSServer(&cert);

unsigned long DUCKDNS_UPDATE_TIME = millis();

void setup() {
  Serial.begin(115200);
  Serial.println("\n");

  for(int i=0; i<NUM_RELAYS; i++) {
    pinMode(relaysGPIO[i].gpio, OUTPUT);
    digitalWrite(relaysGPIO[i].gpio, HIGH);
  }
  
  spiffsInitialization();
  wifiConnection(WIFI_SSID, WIFI_PASS);
  dynamicDNSInitialization(DUCKDNS_DOMAIN, DUCKDNS_TOKEN);
  dynamicDNSUpdate(DUCKDNS_UPDATE_INTERVAL);

  secureServer.registerNode(new ResourceNode("/", "GET", &handleIndex));
  secureServer.registerNode(new ResourceNode("/handlers.js", "GET", &handleScript));
  secureServer.registerNode(new ResourceNode("/api/status", "GET", &handleRelaysStatus));
  secureServer.registerNode(new ResourceNode("/api/update", "POST", &handleRelayUpdate));
  secureServer.setDefaultNode(new ResourceNode("", "GET", &handle404));

  secureServer.addMiddleware(&middlewareAuthentication);

  serverStart(&secureServer);
}

void loop() {
  dynamicDNSUpdateLoop(&DUCKDNS_UPDATE_TIME, DUCKDNS_UPDATE_INTERVAL);
  secureServer.loop();
  delay(1);
}
