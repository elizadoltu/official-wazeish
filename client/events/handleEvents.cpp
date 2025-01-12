#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h>
#include <algorithm>
#include "handleEvents.h"
#include <limits>

void reportEvent(int client_socket) {
    std::cout << "\n=== Reporting Event ===\n";
    std::string event;

    std::cout << "Enter the name of the event and street\n";
    std::cout << "Example: Event [event_name] on street [street_name]\n";
    std::cout << "Type here: ";
    std::cin.ignore();
    std::getline(std::cin, event);

    if (write(client_socket, event.c_str(), event.length()) < 0) {
        perror("Error writing event information to the server");
        return;
    }

    std::cout << "Server response:\n";

    double speed;
    std::cout << "Enter your current speed: ";
    std::cin >> speed;

    // Convert speed to string
    std::string speedStr = std::to_string(speed);

    // Use `write` instead of `send`
    if (write(client_socket, speedStr.c_str(), speedStr.length()) < 0) {
        perror("Error writing speed to the server");
        return;
    }
}
