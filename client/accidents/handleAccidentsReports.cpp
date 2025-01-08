#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h>
#include <algorithm>
#include "handleAccidentsReports.h"

void reportAccidents(int client_socket) {
    std::cout << "\n=== Reporting An Accident ===\n";
    std::string street;

    std::cout << "Enter the name of the street\n";
    std::cout << "Example: Street [street_name]\n";
    std::cout << "Type here: ";
    std::cin.ignore(); // Ignore any leftover newline character
    std::getline(std::cin, street); // Read the full line, including spaces

    // Send the full street name to the server
    send(client_socket, street.c_str(), street.length(), 0);

    char responseBuffer[1024] = {0};
    ssize_t bytes_received = recv(client_socket, responseBuffer, sizeof(responseBuffer) - 1, 0);

    if (bytes_received > 0) {
        responseBuffer[bytes_received] = '\0';
        std::cout << "Server response: " << responseBuffer << std::endl;

        double speed;
        std::cout << "Enter your current speed: ";
        std::cin >> speed;

        std::string speedStr = std::to_string(speed);
        send(client_socket, speedStr.c_str(), speedStr.length(), 0);

        memset(responseBuffer, 0, sizeof(responseBuffer));

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
