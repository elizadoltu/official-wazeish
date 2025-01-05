#pragma once 

#ifndef SERVER_DATA_GASSTATIONS_H
#define SERVER_DATA_GASSTATIONS_H

#include <iostream>
#include <fstream>
#include <json-c/json.h>
#include <string>
#include <vector>
#include <algorithm>

extern json_object* loadGasStationsData();
extern void saveGasStationsData(json_object* gasStationsData);
extern json_object* findGasStationByName(json_object* gasStationsData, const std::string& name);
extern void handleGasPriceChange(int client_socket, const std::string& data);

#endif // SERVER_DATA_GASSTATIONS_H