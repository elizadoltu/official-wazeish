#pragma once 

#ifndef HANDLE_WEATHER_CHANGE
#define HANDLE_WEATHER_CHANGE

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h> 

extern void reportWeatherChange(int client_socket);

#endif