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
#include <limits>

void reportAccidents(int client_socket) {
    std::cout << "\n=== Reporting An Accident ===\n";
    std::string street;

    std::cout << "Enter the name of the street\n";
    std::cout << "Example: Street [street_name]\n";
    std::cout << "Type here: ";
    std::cin.ignore();
    std::getline(std::cin, street);

    if (write(client_socket, street.c_str(), street.length()) < 0) {
        perror("Error writing street name to the server");
        return;
    }
    std::cout << "Server response:\n";

    double speed;
    std::cout << "Enter your current speed: ";
    std::cin >> speed;

    // Convert speed to string
    std::string speedStr = std::to_string(speed);

    if (write(client_socket, speedStr.c_str(), speedStr.length()) < 0) {
        perror("Error writing speed to the server");
        return;
    }
}
