#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <jansson.h>
#include "clientHeader.h"

struct ClientInfo *clientList = NULL; 

struct ClientInfo *createClientNode(const char *city, double speed, int clientId, int clientSocket,
                    bool chooseAccidentsReports, bool chooseEventReports, bool chooseGasStationsReports, bool chooseWeatherReports) {

        struct ClientInfo *newNode = (struct ClientInfo *)malloc(sizeof(struct ClientInfo));
        if (newNode != NULL) {
            newNode->city = strdup(city);
            newNode->speed = speed;
            newNode->clientId = clientId;
            newNode->clientSocket = clientSocket;
            newNode->chooseAccidentsReports = chooseAccidentsReports;
            newNode->chooseEventReports = chooseEventReports;
            newNode->chooseGasStationsReports = chooseGasStationsReports;
            newNode->chooseWeatherReports = chooseWeatherReports;
            newNode->next = NULL; 
        }
        return newNode;
}

void addClientToList(const char *city, double speed, int clientId, int clientSocket,
                    bool chooseAccidentsReports, bool chooseEventReports, bool chooseGasStationsReports, bool chooseWeatherReports) {

        struct ClientInfo *newNode = createClientNode(city, speed, clientId, clientSocket ,chooseAccidentsReports, chooseEventReports, chooseGasStationsReports, chooseWeatherReports);
        if (newNode != NULL) {
            newNode->next = clientList; 
            clientList = newNode;
        }
}

void freeClientList() {

    struct ClientInfo *current = clientList;
    while (current != NULL) {
        struct ClientInfo *next = current->next;
        free((void *)current->city);
        free((void *)current->chooseAccidentsReports);
        free((void *)current->chooseEventReports);
        free((void *)current->chooseGasStationsReports);
        free((void *)current->chooseWeatherReports);
    }
    free(current);
}

void broadcastMessage(const char *message, int senderSocketId, const char *city) {
    struct ClientInfo *current = clientList;
    while (current != NULL) {
        if ((city == NULL || (city != NULL && strcmp(current -> city
                , city) == 0)) && current->clientSocket != senderSocketId) {
                    printf("[server]Sending message to client %d: %s\n", current->clientId, message);
                    send(current->clientSocket, message, strlen(message), 0);
        }
        current = current -> next;
    }
}

void removeClient(int clientId) {
    
    struct ClientInfo *current = clientList;
    struct ClientInfo *prev = NULL; 

    // ITERATE THROUGH THE LIST
    while (current != NULL && current->clientId != clientId) {
        prev = current;
        current = current->next;
    }

    // IF THE CLIENT WAS FOUND 
    if (current != NULL) {
        // UPDATE POINTERS
        if (prev != NULL) {
            prev->next = current->next;
        } else {
            // IF THE FIRST NODE IS THE CLIENT 
            clientList = current->next;
        }

        // FREE THE MEMORY
        free((void *)current->city);
        free(current);
    }
}

const char *getClientByCity(int clientId) {
    struct ClientInfo *current = clientList;

    while (current != NULL) {
        if (current->clientId == clientId) {
            return current->city;
        }
        current = current->next;
    }

    return NULL; // not found
}

int getClientSocketById(int clientId) {
    struct ClientInfo *current = clientList;

    while (current != NULL) {
        if (current->clientId == clientId) {
            return current->clientSocket;
        }
        current = current->next;
    }

    return -1; // not found
}
