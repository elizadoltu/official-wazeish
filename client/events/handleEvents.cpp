#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h>
#include <algorithm>
#include "handleEvents.h"

void reportEvent(int client_socket) {
    std::cout << "\n=== Reporting Event ===\n";
    std::string event; 

    std::cout << "Enter the name of the event and street\n";
    std::cout << "Example: Event [event_name] on street [street_name]\n";
    std::cout << "Type here: ";
    std::cin.ignore();
    std::getline(std::cin, event);

    send(client_socket, event.c_str(), event.length(), 0);

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