#pragma once

#ifndef SAVECLIENTDATA_H
#define SAVECLIENTDATA_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h> 

extern std::vector<int> parseOptions(const std::string &input);
extern void sendClientData(int clientId, int clientSocket, double speed, bool chooseAccidentsReports, bool chooseEventReports, bool chooseGasStationsReports, bool chooseWeatherReports);

#endif
