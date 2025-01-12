#include "WazeishUtils.h"
#include <json-c/json.h>
#include <sys/socket.h>
#include <sys/types.h>
json_object *clientDataObj = json_object_new_object();

void sendClientData(int clientId, int clientSocket, std::string city, double speed,bool chooseAccidentsReports, bool chooseEventReports, bool chooseGasStationsReports, bool chooseWeatherReports) {
    
     // Using json_t from jansson
    json_object_object_add(clientDataObj, "city", json_object_new_string(city.c_str()));
    json_object_object_add(clientDataObj, "clientId", json_object_new_int(clientId));
    json_object_object_add(clientDataObj, "clientSocket", json_object_new_int(clientSocket));
    json_object_object_add(clientDataObj, "speed", json_object_new_double(speed));
    json_object_object_add(clientDataObj, "accidents reports", json_object_new_boolean(chooseAccidentsReports));
    json_object_object_add(clientDataObj, "event reports", json_object_new_boolean(chooseEventReports));
    json_object_object_add(clientDataObj, "gas stations", json_object_new_boolean(chooseGasStationsReports));
    json_object_object_add(clientDataObj, "weather", json_object_new_boolean(chooseWeatherReports));

    const char *jsonData = json_object_to_json_string(clientDataObj);

    // Duplicate the string for returning
    char message[1024];
    send(clientSocket, jsonData, strlen(jsonData), 0);
    // RECEIVE THE CITY NAME FROM THE SERVER
    free(clientDataObj);
    close(clientSocket);

}
