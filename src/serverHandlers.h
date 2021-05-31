#ifndef SERVERHANDLERS_H
#define SERVERHANDLERS_H

#include <HTTPSServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <ArduinoJson.h>
#include "helpers.h"
#include "constants.h"

void serverStart(httpsserver::HTTPSServer * secureServer);

void middlewareAuthentication(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res, std::function<void()> next);

void handleIndex(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res);
void handleScript(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res);
void handleRelaysStatus(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res);
void handleRelayUpdate(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res);
void handle404(httpsserver::HTTPRequest * req, httpsserver::HTTPResponse * res);

#endif
