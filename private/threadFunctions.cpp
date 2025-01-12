#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <cstdint>
#include <jansson.h>
#include <iostream>
#include "clientHeader.h"
#include "threadFunctions.h"
#include "../private/clientHeader.cpp"
#include "../private/isJSON.h"
#include "../private/isDouble.h"
#include "../private/isInteger.h"
#include "../private/isString.h"
#include "../private/isStringDouble.h"

// Mutex for thread safety
struct clientSpeed {
    int clientSocket;
    int clientID;
};
// Handle the client data that the server receives
void *handleClientDataThread(void *arg) {

    char *jsonString = (char *)arg;
    int clientSocket;

    // Parse the json-formatted string
    json_t *clientJsonFile = json_loads(jsonString, 0, NULL);

    if (clientJsonFile != NULL) {

        // Extract the info
        const char *city = json_string_value(json_object_get(clientJsonFile, "city"));
        int clientId = json_integer_value(json_object_get(clientJsonFile, "clientId"));
        clientSocket = json_integer_value(json_object_get(clientJsonFile, "clientSocket"));
        double speed = json_real_value(json_object_get(clientJsonFile, "speed"));
        bool chooseAccidentsReports = json_boolean_value(json_object_get(clientJsonFile, "accidents reports"));
        bool chooseEventReports = json_boolean_value(json_object_get(clientJsonFile, "event reports"));
        bool chooseGasStationReports = json_boolean_value(json_object_get(clientJsonFile, "gas stations"));
        bool chooseWeatherReports = json_boolean_value(json_object_get(clientJsonFile, "weather"));
        // Lock the mutex for thread safety when modifying the linked list

        // Add client information to the linked list
        addClientToList(city, speed, clientId, clientSocket, chooseAccidentsReports, chooseEventReports, chooseGasStationReports, chooseWeatherReports);

        // Unlock the mutex

        // Process the information
        std::cout << "[server]Received data from client: " << clientId << '\n';
        std::cout << "Client Id: " << clientId << '\n';
        std::cout << "Client Socket: " << clientSocket << '\n';
        std::cout << "City: " << city << '\n';
        std::cout << "Speed: " << speed << '\n';
        std::cout << "Accidents Reports: " << chooseAccidentsReports << '\n';
        std::cout << "Event Reports: " << chooseEventReports << '\n';
        std::cout << "Gas Stations Prices: " << chooseGasStationReports << '\n';
        std::cout << "Weather Updates: " << chooseWeatherReports << '\n';
        // Check the speed if it is above 50 (normal speed in cities)
        if (speed > 50) {
            std::cout << "[server]This client " << clientId << " has speed above 50" << '\n';
            send(clientSocket, jsonString, strlen(jsonString), 0);
        }
        // Clean the JSON object
        json_decref(clientJsonFile);
    } else {
        fprintf(stderr, "Error parsing JSON data\n");
    }
    pthread_exit(NULL);
    close(clientSocket);
    free(jsonString);

}

void *handleClientData(void *arg) {
    struct ClientArgs *clientArgs = (struct ClientArgs *)arg;
    int clientSocket = clientArgs->clientSocket;
    char *buffer = clientArgs->buffer;

    if (isdigit(buffer[0]) || buffer[0] == '-' || buffer[0] == '.') {
        // Handle numerical data
        int clientId = atoi(buffer);
        std::cout << "Received integer: " << clientId << '\n';

        // Dynamically allocate a struct clientSpeed and populate it
        struct clientSpeed clientInfo;
        clientInfo.clientSocket = clientSocket;
        clientInfo.clientID = clientId;

        // Call handleRenderCity directly with the prepared struct
        handleRenderCity((void *)&clientInfo);
    } else if (isJSON(buffer)) {
        // Handle JSON data
        std::cout << "Received JSON: " << buffer << '\n';
        // Call your JSON data handling function here
        handleClientDataThread((void *)strdup(buffer));
    } else if (strcmp(buffer, "Disconnected") == 0) {
        // Handle disconnection
        std::cout << "Client disconnected." << '\n';
    } else {
        std::cout << "Unknown data type received: " << buffer << '\n';
    }

    close(clientSocket);
    free(clientArgs);
    return NULL;
}


void *handleRenderCity(void *arg) {
    std::cout << "[server]Am intrat" << '\n';
    struct clientSpeed *clientInfoId = (struct clientSpeed *)arg;

    int clientSocket = clientInfoId->clientSocket;
    int clientID = clientInfoId->clientID;

    // Corrected code
    const char* city = getClientByCity(clientID);

    if (city != NULL) {

        if (clientSocket != -1) {
            send(clientSocket, city, strlen(city), 0);
        } else {
            return NULL;
        }

    } else {
        return NULL;
    }
    close(clientSocket);
    free(arg);
    pthread_exit(NULL);

}
