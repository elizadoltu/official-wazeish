#include "gasStations.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <json-c/json.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/socket.h>

// LOAD THE GAS STATIONS DATA FROM JSON FILE
json_object* loadGasStationsData() {
    std::ifstream file("gasStations.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open gasStationis.json" << std::endl;
        return nullptr;
    }

    std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    json_object* jsonObj = json_tokener_parse(fileContents.c_str());
    return jsonObj;
}

// SAVE THE UPDATED GAS STATIONS DATA TO THE JSON FILE
void saveGasStationsData(json_object* gasStationData) {
    std::ofstream file("gasStations.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open gasStations.json for writing" << std::endl;
        return;
    }
    const char* updateJsonStr = json_object_to_json_string(gasStationData);
    file << updateJsonStr;
    file.close();
}

// FIND GAS STATION BY NAME
json_object* findGasStationByName(json_object* gasStationData, const std::string& name) {
    json_object* gasStations = json_object_object_get(gasStationData, "gas station");
    int numGasStations = json_object_array_length(gasStations);

    for (int i = 0; i < numGasStations; ++i) {
        json_object* gasStation = json_object_array_get_idx(gasStations, i);
        const char* stationName = json_object_get_string(json_object_object_get(gasStation, "name"));

        if (name == stationName) {
            return gasStation;
        }
    }

    return nullptr;
}

// HANDLE THE GAS PRICE CHANGE REPORT 
void handleGasPriceChange(int client_socket, const std::string& data) {
    // Parse the comma-separated values
    std::istringstream stream(data);
    std::string stationName;
    double gasolinePrice, dieselPrice;

    // Parse the station name and prices
    std::getline(stream, stationName, ',');
    stream >> gasolinePrice;
    stream.clear(); // Clear the error state
    stream.ignore(1, ',');  // Ignore the comma
    stream >> dieselPrice;

    // LOAD DATA
    json_object* gasStationData = loadGasStationsData();
    if (!gasStationData) {
        std::cerr << "Failed to load gas stations data" << std::endl;
        return;
    }

    // FIND GAS STATION BY NAME
    json_object* gasStation = findGasStationByName(gasStationData, stationName);
    if (gasStation) {
        // UPDATE PRICES
        json_object* prices = json_object_array_get_idx(json_object_object_get(gasStation, "prices"), 0);
        json_object_object_add(prices, "gasoline", json_object_new_double(gasolinePrice));
        json_object_object_add(prices, "diesel", json_object_new_double(dieselPrice));

        // SAVE UPDATED DATA
        saveGasStationsData(gasStationData);

        // Send confirmation message
        std::string confirmationMessage = "Gas price updated for " + stationName + ". Gasoline: " + std::to_string(gasolinePrice) + ", Diesel: " + std::to_string(dieselPrice);
        send(client_socket, confirmationMessage.c_str(), confirmationMessage.length(), 0);
    } else {
        // Gas station not found, send error message
        std::string errorMessage = "Gas Station not found: " + stationName;
        send(client_socket, errorMessage.c_str(), errorMessage.length(), 0);
    }

    // Cleanup
    json_object_put(gasStationData);
}

