#include "serverHandlers.h"

void serverStart(HTTPSServer * secureServer) {
    Serial.print("Starting server...");
    secureServer->start();
    if (secureServer->isRunning()) {
        Serial.println(" Done!");
    }
}

void middlewareAuthentication(HTTPRequest * req, HTTPResponse * res, std::function<void()> next) {
    if (req->getBasicAuthUser() == USERNAME && req->getBasicAuthPassword() == PASSWORD) {
        next();
    } else {
        res->setStatusCode(401);
        res->setStatusText("Unauthorized");
        res->setHeader("Content-Type", "text/plain");
        res->setHeader("WWW-Authenticate", "Basic realm=\"ESP32 privileged area\"");
    }
}

void handleIndex(HTTPRequest * req, HTTPResponse * res) {
    try {
        res->setHeader("Content-Type", "text/html");
        setResponseContent(res, "/index.html");
    } catch (int e) {
        res->setStatusCode(500);
    }
}

void handleScript(HTTPRequest * req, HTTPResponse * res) {
    try {
        res->setHeader("Content-Type", "application/javascript");
        setResponseContent(res, "/handlers.js");
    } catch (int e) {
        res->setStatusCode(500);
    }
}

void handleRelaysStatus(HTTPRequest * req, HTTPResponse * res) {
    try {
        StaticJsonDocument<RELAYS_STATUS_JSON_SIZE> doc;
        JsonArray arr = doc.to<JsonArray>();

        for(int i = 0; i < NUM_RELAYS; i++) {
            JsonObject relayObj = arr.createNestedObject();
            Relay relay = relaysGPIO[i];
            relayObj["id"] = i;
            relayObj["gpio"] = relay.gpio;
            relayObj["name"] = relay.name;
            relayObj["value"] = !(bool) digitalRead(relay.gpio);
        }

        res->setHeader("Content-Type", "application/json");
        serializeJson(arr, *res);
    } catch (int e) {
        res->setStatusCode(500);
    }
}

void handleRelayUpdate(HTTPRequest * req, HTTPResponse * res) {
    try {
        byte buffer[RELAY_UPDATE_JSON_SIZE];
        req->readBytes(buffer, RELAY_UPDATE_JSON_SIZE);

        StaticJsonDocument<RELAY_UPDATE_JSON_SIZE> doc;
        deserializeJson(doc, buffer);

        int relayId = doc["id"];
        uint8_t relayState = doc["value"] ? LOW : HIGH;
        digitalWrite(relaysGPIO[relayId].gpio, relayState);

        res->setStatusCode(200);
    } catch (int e) {
        res->setStatusCode(500);
    }
}

void handle404(HTTPRequest * req, HTTPResponse * res) {
    res->setStatusCode(404);
}
