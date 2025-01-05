#include "saveClientData.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h> 
#include <sys/socket.h>
#include <sys/types.h>
json_object *clientDataObj = json_object_new_object();

std::vector<int> parseOptions(const std::string &input){
    std::vector<int> options;
    std::stringstream ss(input);
    std::string item; 

    while (std::getline(ss, item, ',')) {
        if (!item.empty()) {
            int option = std::atoi(item.c_str());
            if (option > 0) {
                options.push_back(option);
            } else {
                std::cerr << "Invalid option: " << item << std::endl;
            }
        }
    }

    return options;
}

void sendClientData(int clientId, int clientSocket, double speed, bool chooseAccidentsReports, bool chooseEventReports, bool chooseGasStationsReports, bool chooseWeatherReports){
    
    json_object_object_add(clientDataObj, "clientId", json_object_new_int(clientId));
    json_object_object_add(clientDataObj, "clientSocket", json_object_new_int(clientSocket));
    json_object_object_add(clientDataObj, "speed", json_object_new_double(speed));
    json_object_object_add(clientDataObj, "accidentsReports", json_object_new_boolean(chooseAccidentsReports));
    json_object_object_add(clientDataObj, "eventReports", json_object_new_boolean(chooseEventReports));
    json_object_object_add(clientDataObj, "gasStationsReports", json_object_new_boolean(chooseGasStationsReports));
    json_object_object_add(clientDataObj, "weatherReports", json_object_new_boolean(chooseWeatherReports));

    // Convert JSON object to string
    const char *jsonData = json_object_to_json_string(clientDataObj);

    // Send JSON data to server
    send(clientSocket, jsonData, strlen(jsonData), 0);
    std::cout << "User data sent to server: " << jsonData << std::endl;

    // Free the JSON object
    json_object_put(clientDataObj);
}