#ifndef SERVERHANDLERS_H
#define SERVERHANDLERS_H

#include <HTTPSServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <ArduinoJson.h>
#include "helpers.h"
#include "constants.h"

using namespace httpsserver;

void serverStart(HTTPSServer * secureServer);

void middlewareAuthentication(HTTPRequest * req, HTTPResponse * res, std::function<void()> next);

void handleIndex(HTTPRequest * req, HTTPResponse * res);
void handleScript(HTTPRequest * req, HTTPResponse * res);
void handleRelaysStatus(HTTPRequest * req, HTTPResponse * res);
void handleRelayUpdate(HTTPRequest * req, HTTPResponse * res);
void handle404(HTTPRequest * req, HTTPResponse * res);

#endif
