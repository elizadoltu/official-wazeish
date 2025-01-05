#ifndef CLIENT_HEADER_H
#define CLIENT_HEADER_H

struct ClientInfo {
    const char *city; 
    double speed; 
    int clientId;
    int clientSocket;
    bool chooseAccidentsReports;
    bool chooseEventReports;
    bool chooseGasStationsReports;
    bool chooseWeatherReports;

    struct ClientInfo *next; 
};

extern struct ClientInfo *clientList;

struct ClientArgs {
    int clientSocket;
    char buffer[1024];
};

struct ClientInfo *createClientNode(const char *city, double speed, int clientId, int clientSocket, 
                                    bool chooseAccidentsReports, bool chooseEventReports, 
                                    bool chooseGasStationsReports, bool chooseWeatherReports);
void addClientToList(const char *city, double speed, int clientId, int clientSocket,
                     bool chooseAccidentsReports, bool chooseEventReports, 
                     bool chooseGasStationsReports, bool chooseWeatherReports);
void broadcastMessage(const char *message, int senderSockedId);
void freeClientList();
void removeClient(int clientId);
const char *getClientByCity(int clientId);
int getClientSocketById(int clientId);

#endif
