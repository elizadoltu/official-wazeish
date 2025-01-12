#pragma once 

#ifndef ACCIDENTS_H
#define ACCIDENTS_H

#include <iostream>
#include <fstream>
#include <json-c/json.h>
#include <string>
#include <vector>
#include <algorithm>

extern json_object* loadStreetsData();
extern bool isStreetNameAvailable(json_object* streets, const std::string& name);
extern void handleAccidentsReports(int client_socket, const std::string& data);

#endif