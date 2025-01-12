#include <iostream>
#include <fstream>
#include <sstream>
#include <json-c/json.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include "events.h"

// HANDLE THE EVENT REPORTS
void handleEventReports(int client_socket, const std::string& data) {
    std::string report = data;
    std::cout << "Received event report: " << report << std::endl;

    std::string message = "New event report: " + report + ". Enter your current speed:";
    write(client_socket, message.c_str(), message.length());

    char buffer[1024] = {0};
    ssize_t bytes_received = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_received <= 0) {
        std::cerr << "Error receiving speed from client!" << std::endl;
        return;
    }

    buffer[bytes_received] = '\0';
    double clientSpeed = std::stod(buffer);

    // RESPOND BASED ON SPEED
    std::string response = (clientSpeed > 30) ? "Slow down! Your speed exceeds the limit." : "Your speed is good. Drive safely!";
    write(client_socket, response.c_str(), response.length());
}
