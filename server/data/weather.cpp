#include "weather.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <json-c/json.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/socket.h>

// LOAD SPEED LIMITS DATA
json_object* loadSpeedLimitsData() {
    std::ifstream file("speedLimits.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open speedLimits.json" << std::endl;
        return nullptr;
    }

    std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    json_object* jsonObj = json_tokener_parse(fileContents.c_str());
    return jsonObj;
}

// GET THE SPEED LIMIT FOR A SPECIFIC WEATHER CONDITION
double findSpeedLimitByWeather(json_object* speedLimits, const std::string& weather) {
    json_object* limitsArray = nullptr;
    if (!json_object_object_get_ex(speedLimits, "limits", &limitsArray)) {
        std::cerr << "Failed to find 'limits' array in the JSON object!" << std::endl;
        return -1;
    }

    json_object* limitsObj = json_object_array_get_idx(limitsArray, 0);
    if (!limitsObj) {
        std::cerr << "Failed to access limits object in the array!" << std::endl;
        return -1;
    }

    json_object* speedLimitObject = nullptr;
    if (json_object_object_get_ex(limitsObj, weather.c_str(), &speedLimitObject)) {
        return json_object_get_double(speedLimitObject);
    }

    std::cerr << "Weather condition '" << weather << "' not found in JSON!" << std::endl;
    return -1;
}

void handleWeatherChange(int client_socket, const std::string& data) {
    // Load the speed limits data
    json_object* speedLimits = loadSpeedLimitsData();
    if (!speedLimits) {
        std::cerr << "Failed to load speed limits data!" << std::endl;
        const char* errorMsg = "Error: Unable to load speed limits data!";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
        return;
    }

    // Parse the weather condition received from the client
    std::string weather = data; // Assuming 'data' contains the weather condition as a string
    std::cout << "Received weather condition: " << weather << std::endl;

    // Find the speed limit for the given weather condition
    double speedLimit = findSpeedLimitByWeather(speedLimits, weather);
    if (speedLimit < 0) {
        const char* errorMsg = "Error: Invalid weather condition or speed limits data!";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
        json_object_put(speedLimits); // Free the JSON object
        return;
    }

    // Inform the client about the speed limit and ask for their speed
    std::ostringstream message;
    message << "The speed limit for " << weather << " weather is " << speedLimit << " km/h. Enter your current speed:";
    std::string messageStr = message.str();
    send(client_socket, messageStr.c_str(), messageStr.length(), 0);

    // Receive the client's speed
    char buffer[1024] = {0};
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cerr << "Error: Failed to receive speed from the client!" << std::endl;
        json_object_put(speedLimits); // Free the JSON object
        return;
    }

    buffer[bytes_received] = '\0'; // Null-terminate the received data
    double clientSpeed = std::stod(buffer); // Convert the received speed to double

    // Compare the client's speed with the speed limit and send a response
    std::string response;
    if (clientSpeed > speedLimit) {
        response = "Slow down! Your speed exceeds the limit.";
    } else {
        response = "Your speed is good. Drive safely!";
    }

    send(client_socket, response.c_str(), response.length(), 0);

    // Clean up
    json_object_put(speedLimits); // Free the JSON object
}
