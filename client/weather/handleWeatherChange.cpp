#include "handleWeatherChange.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h>
#include <algorithm>

void reportWeatherChange(int client_socket) {
    std::cout << "\n=== Reporting Weather Change ===\n";
    std::string weather;

    std::cout << "Enter the current weather\n";
    std::cout << "1. Sunny\n";
    std::cout << "2. Rainy\n";
    std::cout << "3. Snowy\n";
    std::cout << "4. Windy\n";
    std::cout << "Type here: ";
    std::cin >> weather;

    // Send the selected weather option to the server
    send(client_socket, weather.c_str(), weather.length(), 0);

    char responseBuffer[1024] = {0}; // Initialize with null characters
    ssize_t bytes_received = recv(client_socket, responseBuffer, sizeof(responseBuffer) - 1, 0);

    if (bytes_received > 0) {
        responseBuffer[bytes_received] = '\0';  // Null-terminate the response
        std::cout << "Server response: " << responseBuffer << std::endl;

        double speed; 
        std::cout << "Enter your current speed: ";
        std::cin >> speed;

        // Convert the speed (double) to a string
        std::string speedStr = std::to_string(speed);

        // Send the speed string to the server
        send(client_socket, speedStr.c_str(), speedStr.length(), 0);

        // Clear the buffer manually by overwriting it with null characters
        for (int i = 0; i < 1024; i++) {
            responseBuffer[i] = '\0';
        }

        bytes_received = recv(client_socket, responseBuffer, sizeof(responseBuffer) - 1, 0);

        if (bytes_received > 0) {
            responseBuffer[bytes_received] = '\0';
            std::cout << "Server response: " << responseBuffer << std::endl;
        } else {
            std::cerr << "Error receiving data from server!" << std::endl;
        }
    } else {
        std::cerr << "Error receiving data from server!" << std::endl;
    }
}

