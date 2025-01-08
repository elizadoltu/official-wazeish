#include "accidents.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <json-c/json.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/socket.h>

// GET STREETS DATA
json_object* loadStreetsData() {
    std::ifstream file("cities.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open cities.json" << std::endl;
        return nullptr;
    }

    std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    json_object* jsonObj = json_tokener_parse(fileContents.c_str());
    return jsonObj;
}

// RETURN TRUE IF THE STREET IT IS IN THE DATABASE, FALSE OTHERWISE
bool isStreetNameAvailable(json_object* streets, const std::string& name) {
    json_object* streetsArray = nullptr;

    if (!json_object_object_get_ex(streets, "streets", &streetsArray)) {
        std::cerr << "Failed to find 'streets' array in the JSON object!" << std::endl;
        return false;
    }

    size_t arrayLength = json_object_array_length(streetsArray);
    for (size_t i = 0; i < arrayLength; ++i) {
        json_object* streetObj = json_object_array_get_idx(streetsArray, i);
        if (!streetObj) {
            std::cerr << "Failed to access street object at index " << i << " in the array!" << std::endl;
            continue;
        }

        json_object* streetNameObj = nullptr;
        if (json_object_object_get_ex(streetObj, "street_name", &streetNameObj)) {
            const char* streetName = json_object_get_string(streetNameObj);
            if (streetName && name == streetName) {
                return true;
            }
        }
    }

    return false;
}

// HANDLE THE ACCIDENTS REPORTS
void handleAccidentsReports(int client_socket, const std::string& data) {
    json_object* streets = loadStreetsData();
    if (!streets) {
        const char* errorMsg = "Error: Unable to load streets data!";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
        return;
    }

    // Remove "Street " prefix if present
    std::string streetName = data;
    if (streetName.find("Street ") == 0) {
        streetName = streetName.substr(7);
    }

    std::cout << "Received accident report on: " << streetName << std::endl;

    // Validate street name
    bool foundStreet = isStreetNameAvailable(streets, streetName);
    if (!foundStreet) {
        const char* errorMsg = "Error: Invalid street name!";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
        json_object_put(streets);
        return;
    }

    // Respond and request speed
    std::string message = "New accident report on " + streetName + ". Enter your current speed:";
    send(client_socket, message.c_str(), message.length(), 0);

    // Receive speed
    char buffer[1024] = {0};
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cerr << "Error receiving speed from client!" << std::endl;
        json_object_put(streets);
        return;
    }

    buffer[bytes_received] = '\0';
    double clientSpeed = std::stod(buffer);

    // Respond based on speed
    std::string response = (clientSpeed > 20) ? "Slow down! Your speed exceeds the limit." : "Your speed is good. Drive safely!";
    send(client_socket, response.c_str(), response.length(), 0);

    json_object_put(streets);
}
