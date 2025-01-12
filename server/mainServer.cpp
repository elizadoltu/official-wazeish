    #include <iostream>
    #include <cstring>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <cstdlib>
    #include <json-c/json.h>
    #include <fstream>
    #include "data/gasStations.cpp"
    #include "data/weather.cpp"
    #include "data/accidents.cpp"
    #include "data/events.cpp"
    #include <mutex>
    #include <vector>
#include <bits/this_thread_sleep.h>

    #define PORT 1168
    #define MAX_CONNECTIONS 20

    std::vector<int> clientSockets;
    pthread_mutex_t clientSocketsMutex = PTHREAD_MUTEX_INITIALIZER;

bool checkWeatherReports(int clientId) {
    // Open the JSON file
    std::ifstream file("users.json");
    if (!file.is_open()) {
        std::cerr << "Error opening file users.json" << std::endl;
        return false;
    }

    // Read the entire file into a string
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Parse the JSON content
    struct json_object *parsed_json = json_tokener_parse(jsonContent.c_str());
    if (parsed_json == nullptr) {
        std::cerr << "Error parsing JSON file." << std::endl;
        return false;
    }

    // Check if the parsed object is an array
    if (!json_object_is_type(parsed_json, json_type_array)) {
        std::cerr << "JSON data is not an array." << std::endl;
        json_object_put(parsed_json);
        return false;
    }

    // Iterate over the array
    size_t array_len = json_object_array_length(parsed_json);
    for (size_t i = 0; i < array_len; i++) {
        struct json_object *entry = json_object_array_get_idx(parsed_json, i);

        // Get clientId
        struct json_object *json_clientId;
        if (json_object_object_get_ex(entry, "clientId", &json_clientId) &&
            json_object_get_int(json_clientId) == clientId) {
            
            // Get weatherReports
            struct json_object *json_weatherReports;
            if (json_object_object_get_ex(entry, "weatherReports", &json_weatherReports)) {
                bool result = json_object_get_boolean(json_weatherReports);
                json_object_put(parsed_json); 
                return result;
            }
        }
    }

    json_object_put(parsed_json); 
    return false;
}

bool checkEventReports(int clientId) {
    // Open the JSON file
    std::ifstream file("users.json");
    if (!file.is_open()) {
        std::cerr << "Error opening file users.json" << std::endl;
        return false;
    }

    // Read the entire file into a string
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Parse the JSON content
    struct json_object *parsed_json = json_tokener_parse(jsonContent.c_str());
    if (parsed_json == nullptr) {
        std::cerr << "Error parsing JSON file." << std::endl;
        return false;
    }

    // Check if the parsed object is an array
    if (!json_object_is_type(parsed_json, json_type_array)) {
        std::cerr << "JSON data is not an array." << std::endl;
        json_object_put(parsed_json);
        return false;
    }

    // Iterate over the array
    size_t array_len = json_object_array_length(parsed_json);
    for (size_t i = 0; i < array_len; i++) {
        struct json_object *entry = json_object_array_get_idx(parsed_json, i);

        // Get clientId
        struct json_object *json_clientId;
        if (json_object_object_get_ex(entry, "clientId", &json_clientId) &&
            json_object_get_int(json_clientId) == clientId) {
            
            // Get weatherReports
            struct json_object *json_weatherReports;
            if (json_object_object_get_ex(entry, "eventReports", &json_weatherReports)) {
                bool result = json_object_get_boolean(json_weatherReports);
                json_object_put(parsed_json); 
                return result;
            }
        }
    }

    json_object_put(parsed_json); 
    return false;
}

bool checkGasStationReports(int clientId) {
    // Open the JSON file
    std::ifstream file("users.json");
    if (!file.is_open()) {
        std::cerr << "Error opening file users.json" << std::endl;
        return false;
    }

    // Read the entire file into a string
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Parse the JSON content
    struct json_object *parsed_json = json_tokener_parse(jsonContent.c_str());
    if (parsed_json == nullptr) {
        std::cerr << "Error parsing JSON file." << std::endl;
        return false;
    }

    // Check if the parsed object is an array
    if (!json_object_is_type(parsed_json, json_type_array)) {
        std::cerr << "JSON data is not an array." << std::endl;
        json_object_put(parsed_json);
        return false;
    }

    // Iterate over the array
    size_t array_len = json_object_array_length(parsed_json);
    for (size_t i = 0; i < array_len; i++) {
        struct json_object *entry = json_object_array_get_idx(parsed_json, i);

        // Get clientId
        struct json_object *json_clientId;
        if (json_object_object_get_ex(entry, "clientId", &json_clientId) &&
            json_object_get_int(json_clientId) == clientId) {
            
            // Get weatherReports
            struct json_object *json_weatherReports;
            if (json_object_object_get_ex(entry, "gasStationsReports", &json_weatherReports)) {
                bool result = json_object_get_boolean(json_weatherReports);
                json_object_put(parsed_json); 
                return result;
            }
        }
    }

    json_object_put(parsed_json); 
    return false;
}



// Function to broadcast a message to all connected clients except the sender
void broadcastToOtherClients(int senderSocket, int clientId, const std::string &message) {
    std::cout << "Broadcasting message to clients. Sender: " << senderSocket << std::endl;
    for (int clientSocket : clientSockets) {
        std::cout << "Checking client socket: " << clientSocket << std::endl;

        // Skip the sender socket
        if (clientSocket == senderSocket) {
            std::cout << "Skipping sender socket: " << senderSocket << std::endl;
            continue;
        }

        // Check if the client has enabled weather reports
        if (checkWeatherReports(clientSocket)) {
            std::cout << "Weather reports are enabled for clientId: " << clientSocket << std::endl;

            // Send the message to the client
            if (write(clientSocket, message.c_str(), message.length()) == -1) {
                perror("Failed to send message to client");
            } else {
                std::cout << "Message sent to client socket: " << clientSocket << std::endl;
            }
        } else {
            std::cout << "Weather reports are disabled for clientId: " << clientId << std::endl;
        }
    }
}

void broadcastToOtherClientsGasStations(int senderSocket, int clientId, const std::string &message) {
    std::cout << "Broadcasting message to clients. Sender: " << senderSocket << std::endl;
    for (int clientSocket : clientSockets) {
        std::cout << "Checking client socket: " << clientSocket << std::endl;

        // Skip the sender socket
        if (clientSocket == senderSocket) {
            std::cout << "Skipping sender socket: " << senderSocket << std::endl;
            continue;
        }

        // Check if the client has enabled event reports
        if (checkGasStationReports(clientSocket)) {
            std::cout << "Gas station are enabled for clientId: " << clientSocket << std::endl;

            // Send the message to the client
            if (write(clientSocket, message.c_str(), message.length()) == -1) {
                perror("Failed to send message to client");
            } else {
                std::cout << "Message sent to client socket: " << clientSocket << std::endl;
            }
        } else {
            std::cout << "Gas station reports are disabled for clientId: " << clientId << std::endl;
        }
    }
}

void broadcastToOtherClientsEvent(int senderSocket, int clientId, const std::string &message) {
    std::cout << "Broadcasting message to clients. Sender: " << senderSocket << std::endl;
    for (int clientSocket : clientSockets) {
        std::cout << "Checking client socket: " << clientSocket << std::endl;

        // Skip the sender socket
        if (clientSocket == senderSocket) {
            std::cout << "Skipping sender socket: " << senderSocket << std::endl;
            continue;
        }

        // Check if the client has enabled event reports
        if (checkEventReports(clientSocket)) {
            std::cout << "Eventreports are enabled for clientId: " << clientSocket << std::endl;

            // Send the message to the client
            if (write(clientSocket, message.c_str(), message.length()) == -1) {
                perror("Failed to send message to client");
            } else {
                std::cout << "Message sent to client socket: " << clientSocket << std::endl;
            }
        } else {
            std::cout << "Event reports are disabled for clientId: " << clientId << std::endl;
        }
    }
}

void broadcastToOtherClientsAccident(int senderSocket, int clientId, const std::string &message) {
    std::cout << "Broadcasting message to clients. Sender: " << senderSocket << std::endl;
    for (int clientSocket : clientSockets) {
        std::cout << "Checking client socket: " << clientSocket << std::endl;

        // Skip the sender socket
        if (clientSocket == senderSocket) {
            std::cout << "Skipping sender socket: " << senderSocket << std::endl;
            continue;
        }

        // Check if the client has enabled weather reports
        std::cout << "Accident are enabled for clientId: " << clientSocket << std::endl;

        // Send the message to the client
        if (write(clientSocket, message.c_str(), message.length()) == -1) {
            perror("Failed to send message to client");
        } else {
            std::cout << "Message sent to client socket: " << clientSocket << std::endl;
        }
        
    }
}


    struct ClientInServer {
        int clientSocket;
        int clientUniqueId;
    };

    int globalClientId = 0; 
    pthread_mutex_t idMutex = PTHREAD_MUTEX_INITIALIZER; 

    void saveDataToFile(const char *jsonData, int clientId) {
        // Read existing JSON file
        std::ifstream fileIn("users.json");
        json_object *jsonArray;
        if (fileIn.is_open()) {
            std::string content((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
            jsonArray = content.empty() ? json_object_new_array() : json_tokener_parse(content.c_str());
            if (!json_object_is_type(jsonArray, json_type_array)) {
                jsonArray = json_object_new_array();
            }
            fileIn.close();
        } else {
            jsonArray = json_object_new_array();
        }

        // Parse incoming JSON data
        json_object *newUser = json_tokener_parse(jsonData);
        if (!newUser) {
            std::cerr << "Failed to parse JSON data!" << std::endl;
            return;
        }

        json_object *clientIdObj = json_object_new_int(clientId);
        json_object_object_add(newUser, "clientId", clientIdObj);

        // Add the new user to the array
        json_object_array_add(jsonArray, newUser);

        // Write back to file
        std::ofstream fileOut("users.json");
        if (fileOut.is_open()) {
            fileOut << json_object_to_json_string_ext(jsonArray, JSON_C_TO_STRING_PRETTY);
            fileOut.close();
        } else {
            std::cerr << "Error opening users.json for writing!" << std::endl;
        }

        json_object_put(jsonArray);
    }


void *handleClientCommunication(void *arg) {
    struct ClientInServer *clientInfo = (struct ClientInServer *)arg;
    int client_socket = clientInfo->clientSocket;
    int clientId = client_socket;
    char buffer[1024];

    while (true) {
        ssize_t bytes_received = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                std::cout << "Client disconnected.\n";
            } else {
                perror("Receive failed");
            }
            break;
        }

        buffer[bytes_received] = '\0';
        std::cout << "Received data from client: " << buffer << std::endl;
        if (strncmp(buffer, "Street ", 7) == 0) {
            // Extract the street name
            std::string fullMessage(buffer);
            std::string streetName = fullMessage.substr(7); 
            
            std::cout << "Extracted Street Name: " << streetName << std::endl;

            std::string response = "Received street name: " + streetName;
            {
                std::string message = "=== New Accident Report ===\nOn street: " + streetName;

                pthread_mutex_lock(&clientSocketsMutex); 
                broadcastToOtherClientsAccident(client_socket, clientId, message);
                pthread_mutex_unlock(&clientSocketsMutex);
            }
            handleAccidentsReports(client_socket, buffer);

        }

        if (strncmp(buffer, "Event ", 6) == 0) {
            {
                std::string message = "=== New Event Report ===\nReport: " + std::string(buffer);

                pthread_mutex_lock(&clientSocketsMutex); 
                broadcastToOtherClientsEvent(client_socket, clientId, message);
                pthread_mutex_unlock(&clientSocketsMutex);
            }
            handleEventReports(client_socket, buffer);
            
        }

        if (strcmp(buffer, "Sunny") == 0 || strcmp(buffer, "Rainy") == 0 ||
    strcmp(buffer, "Snowy") == 0 || strcmp(buffer, "Windy") == 0)
 {
            std::cout << "Valid weather update received: " << buffer << std::endl;
            std::string message = "=== New Weather Report ===\nWeather: " + std::string(buffer);
            {
                pthread_mutex_lock(&clientSocketsMutex); 
                broadcastToOtherClients(client_socket, clientId, message);
                pthread_mutex_unlock(&clientSocketsMutex);
            }
            handleWeatherChange(client_socket, buffer);
        }
        // IF IT IS IN JSON FORMAT
        else if (buffer[0] == '{' && buffer[bytes_received - 1] == '}') {
            struct json_object *parsed_json = json_tokener_parse(buffer);
            if (parsed_json == NULL) {
                std::cerr << "Failed to parse JSON data!" << std::endl;
                break;
            }

            struct json_object *stationName_obj;
            struct json_object *gasolinePrice_obj;
            struct json_object *dieselPrice_obj;

            if (json_object_object_get_ex(parsed_json, "stationName", &stationName_obj) &&
                json_object_object_get_ex(parsed_json, "gasolinePrice", &gasolinePrice_obj) &&
                json_object_object_get_ex(parsed_json, "dieselPrice", &dieselPrice_obj)) {

                std::string stationName = json_object_get_string(stationName_obj);
                double gasolinePrice = json_object_get_double(gasolinePrice_obj);
                double dieselPrice = json_object_get_double(dieselPrice_obj);
                {
                std::string message = "=== New Gas Prices ===\n For next station: " + stationName;

                pthread_mutex_lock(&clientSocketsMutex); 
                broadcastToOtherClientsGasStations(client_socket, clientId, message);
                pthread_mutex_unlock(&clientSocketsMutex);
                }
                handleGasPriceChange(client_socket, stationName + "," + std::to_string(gasolinePrice) + "," + std::to_string(dieselPrice));
            }
            else {
                struct json_object *speed_obj;
                if (json_object_object_get_ex(parsed_json, "speed", &speed_obj)) {
                    double speed = json_object_get_double(speed_obj);
                    if (speed > 50) {
                        std::string warning_message = "Speed is too much! Slow down!";
                        write(client_socket, warning_message.c_str(), warning_message.length());
                        std::cout << "Sent warning to client: " << warning_message << std::endl;
                    }
                }
                saveDataToFile(buffer, client_socket);
            }

            json_object_put(parsed_json);
        } else {
            std::string data(buffer);
            size_t firstCommaPos = data.find(',');
            size_t secondCommaPos = data.rfind(',');

            if (firstCommaPos != std::string::npos && secondCommaPos != std::string::npos && firstCommaPos != secondCommaPos) {
                std::string stationName = data.substr(0, firstCommaPos);
                double gasolinePrice = std::stod(data.substr(firstCommaPos + 1, secondCommaPos - firstCommaPos - 1));
                double dieselPrice = std::stod(data.substr(secondCommaPos + 1));

                handleGasPriceChange(client_socket, stationName + "," + std::to_string(gasolinePrice) + "," + std::to_string(dieselPrice));
            } else {
                std::cerr << "Invalid data format received!" << std::endl;
            }
        }

        // Send a response to the client
        std::string response = "Server received your data!";
        send(client_socket, response.c_str(), response.length(), 0);
        std::this_thread::sleep_for(std::chrono::seconds(1));  // JUST FOR TESTING AND FOR ME
        std::cout << "Server sleeping for 2 seconds after handling client.\n";
    }

    close(client_socket);
    free(clientInfo); 
    return nullptr;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the server socket to the port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_socket);
        return -1;
    }

    // Start listening for incoming connections
    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        perror("Listen failed");
        close(server_socket);
        return -1;
    }

    std::cout << "[server] Listening on port: " << PORT << std::endl;

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "Client connected.\n";

        // Allocate and initialize client info
        struct ClientInServer *clientInfo = (struct ClientInServer *)malloc(sizeof(struct ClientInServer));
        clientInfo->clientSocket = client_socket;

        // Assign a unique ID to the client
        pthread_mutex_lock(&idMutex);
        clientInfo->clientUniqueId = client_socket;
        pthread_mutex_unlock(&idMutex);

        std::cout << "Assigned Client ID: " << clientInfo->clientUniqueId << std::endl;
        pthread_mutex_lock(&clientSocketsMutex);  
        clientSockets.push_back(client_socket); 
         pthread_mutex_unlock(&clientSocketsMutex);
        // Create a thread for the client
        pthread_t client_thread;
        if (pthread_create(&client_thread, nullptr, handleClientCommunication, (void *)clientInfo) != 0) {
            std::cerr << "Failed to create thread for client" << std::endl;
            free(clientInfo);
        } else {
            pthread_detach(client_thread);
        }
    }

    // Close the server socket 
    close(server_socket);
    return 0;
}