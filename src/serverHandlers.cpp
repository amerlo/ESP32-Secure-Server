#include "serverHandlers.h"

void serverStart(httpsserver::HTTPSServer * secureServer) {
    Serial.print("Starting server...");
    secureServer->start();
    if (secureServer->isRunning()) {
        Serial.println(" Done!");
    }
}

void middlewareAuthentication(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res, std::function<void()> next) {
    if (req->getBasicAuthUser() == USERNAME && req->getBasicAuthPassword() == PASSWORD) {
        next();
    } else {
        res->setStatusCode(401);
        res->setStatusText("Unauthorized");
        res->setHeader("Content-Type", "text/plain");
        res->setHeader("WWW-Authenticate", "Basic realm=\"ESP32 privileged area\"");
    }
}

void handleIndex(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res) {
    try {
        res->setHeader("Content-Type", "text/html");
        setResponseContent(res, "/index.html");
    } catch (int e) {
        res->setStatusCode(500);
    }
}

void handleScript(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res) {
    try {
        res->setHeader("Content-Type", "application/javascript");
        setResponseContent(res, "/handlers.js");
    } catch (int e) {
        res->setStatusCode(500);
    }
}

void handleRelaysStatus(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res) {
    try {
        const size_t jsonSize = JSON_ARRAY_SIZE(NUM_RELAYS) + NUM_RELAYS * JSON_OBJECT_SIZE(6);
        //StaticJsonDocument<jsonSize> doc; --> Should be faster! Test it!
        DynamicJsonDocument doc(jsonSize);
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

void handleRelayUpdate(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res) {
    try {
        const size_t jsonSize = JSON_OBJECT_SIZE(2) + 16;
        //StaticJsonDocument<jsonSize> doc; --> Should be faster! Test it!
        DynamicJsonDocument doc(jsonSize);

        char * buffer = new char[jsonSize + 1];
        memset(buffer, 0, jsonSize + 1);

        size_t idx = 0;
        while (!req->requestComplete() && idx < jsonSize) {
            idx += req->readChars(buffer + idx, jsonSize-idx);
        }

        deserializeJson(doc, buffer);

        delete[] buffer;

        int relayId = doc["id"];
        uint8_t relayState = doc["value"] ? LOW : HIGH;
        digitalWrite(relaysGPIO[relayId].gpio, relayState);

        res->setStatusCode(200);
    } catch (int e) {
        res->setStatusCode(500);
    }
}

void handle404(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res) {
    res->setStatusCode(404);
}
