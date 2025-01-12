#pragma once

#ifndef WEATHER_H 
#define WEATHER_H

#include <iostream>
#include <fstream>
#include <json-c/json.h>
#include <string>
#include <vector>
#include <algorithm>

extern json_object* loadSpeedLimitsData();
extern double findSpeedLimitByWeather(json_object* speedLimits, const std::string& weather);
extern void handleWeatherChange(int client_socket, const std::string& data);

#endif