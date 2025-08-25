#pragma once

#include <Arduino.h>
#include <WebServer.h>

// Declare the web server and its state
extern WebServer server;
extern bool serverRunning;

// Declare the functions to start and stop the web server
void startWebServer();
void stopWebServer();
void Webpage_menu();