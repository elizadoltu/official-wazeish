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

#define PORT 1168
#define MAX_CONNECTIONS 20

struct ClientInServer {
    int clientSocket;
    int clientUniqueId;
};

int globalClientId = 0; 
pthread_mutex_t idMutex = PTHREAD_MUTEX_INITIALIZER; 

void saveDataToFile(const char *jsonData) {
    // Read the existing JSON file into a json_object
    std::ifstream fileIn("users.json");
    json_object *jsonArray;

    if (fileIn.is_open()) {
        std::string content((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
        if (!content.empty()) {
            jsonArray = json_tokener_parse(content.c_str());
            if (!jsonArray || !json_object_is_type(jsonArray, json_type_array)) {
                jsonArray = json_object_new_array(); // Create a new array if parsing fails
            }
        } else {
            jsonArray = json_object_new_array(); // Create a new array if file is empty
        }
        fileIn.close();
    } else {
        jsonArray = json_object_new_array(); // Create a new array if file doesn't exist
    }

    // Parse the incoming JSON data
    json_object *newUser = json_tokener_parse(jsonData);
    if (newUser == NULL) {
        std::cerr << "Failed to parse JSON data!" << std::endl;
        return;
    }

    // Add the new user data to the array
    json_object_array_add(jsonArray, newUser);

    // Write the updated array back to the file with indentation
    std::ofstream fileOut("users.json");
    if (!fileOut.is_open()) {
        std::cerr << "Error opening users.json file for writing!" << std::endl;
        return;
    }
    fileOut << json_object_to_json_string_ext(jsonArray, JSON_C_TO_STRING_PRETTY);
    fileOut.close();

    std::cout << "User data saved to users.json with indentation: " << json_object_to_json_string_ext(jsonArray, JSON_C_TO_STRING_PRETTY) << std::endl;

    // Free the json_object memory
    json_object_put(jsonArray);
}


void *handleClientCommunication(void *arg) {
    struct ClientInServer *clientInfo = (struct ClientInServer *)arg;
    int client_socket = clientInfo->clientSocket;
    char buffer[1024];

    while (true) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                std::cout << "Client disconnected.\n";
            } else {
                perror("Receive failed");
            }
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received data
        std::cout << "Received data from client: " << buffer << std::endl;
        if (strncmp(buffer, "Street ", 7) == 0) {
            // Extract the street name
            std::string fullMessage(buffer);
            std::string streetName = fullMessage.substr(7); // Get the substring after "Street "
            
            // Print the extracted street name
            std::cout << "Extracted Street Name: " << streetName << std::endl;

            // You can process the street name further here, e.g., save it to a file or database
            std::string response = "Received street name: " + streetName;
            handleAccidentsReports(client_socket, buffer);

            continue; // Skip the rest of the logic for this message
        }

        if (strcmp(buffer, "Sunny") == 0 || strcmp(buffer, "Rainy") == 0 ||
    strcmp(buffer, "Snowy") == 0 || strcmp(buffer, "Windy") == 0)
 {
            std::cout << "Valid weather update received: " << buffer << std::endl;
            handleWeatherChange(client_socket, buffer);
        }
        // Check if the data is in JSON format
        else if (buffer[0] == '{' && buffer[bytes_received - 1] == '}') {
            // Parse the incoming data as JSON
            struct json_object *parsed_json = json_tokener_parse(buffer);
            if (parsed_json == NULL) {
                std::cerr << "Failed to parse JSON data!" << std::endl;
                break;
            }

            // Check for a gas price change message
            struct json_object *stationName_obj;
            struct json_object *gasolinePrice_obj;
            struct json_object *dieselPrice_obj;

            // If it's a gas price update request
            if (json_object_object_get_ex(parsed_json, "stationName", &stationName_obj) &&
                json_object_object_get_ex(parsed_json, "gasolinePrice", &gasolinePrice_obj) &&
                json_object_object_get_ex(parsed_json, "dieselPrice", &dieselPrice_obj)) {

                // Extract the values from the JSON
                std::string stationName = json_object_get_string(stationName_obj);
                double gasolinePrice = json_object_get_double(gasolinePrice_obj);
                double dieselPrice = json_object_get_double(dieselPrice_obj);

                // Call the function to handle gas price change
                handleGasPriceChange(client_socket, stationName + "," + std::to_string(gasolinePrice) + "," + std::to_string(dieselPrice));
            }
            // If not a gas price change message, proceed with handling other JSON data (e.g., speed warnings)
            else {
                // Extract the speed value if present
                struct json_object *speed_obj;
                if (json_object_object_get_ex(parsed_json, "speed", &speed_obj)) {
                    double speed = json_object_get_double(speed_obj);
                    if (speed > 50) {
                        std::string warning_message = "Speed is too much! Slow down!";
                        send(client_socket, warning_message.c_str(), warning_message.length(), 0);
                        std::cout << "Sent warning to client: " << warning_message << std::endl;
                    }
                }
                saveDataToFile(buffer);
            }

            // Free the JSON object
            json_object_put(parsed_json);
        } else {
            // Handle the case where the data is in non-JSON format, specifically for gas price change
            std::string data(buffer);
            size_t firstCommaPos = data.find(',');
            size_t secondCommaPos = data.rfind(',');

            if (firstCommaPos != std::string::npos && secondCommaPos != std::string::npos && firstCommaPos != secondCommaPos) {
                // Extract gas station name and prices
                std::string stationName = data.substr(0, firstCommaPos);
                double gasolinePrice = std::stod(data.substr(firstCommaPos + 1, secondCommaPos - firstCommaPos - 1));
                double dieselPrice = std::stod(data.substr(secondCommaPos + 1));

                // Call the function to handle gas price change
                handleGasPriceChange(client_socket, stationName + "," + std::to_string(gasolinePrice) + "," + std::to_string(dieselPrice));
            } else {
                std::cerr << "Invalid data format received!" << std::endl;
            }
        }

        // Send a response to the client
        std::string response = "Server received your data!";
        send(client_socket, response.c_str(), response.length(), 0);
    }

    // Close the client socket
    close(client_socket);
    free(clientInfo); // Free the allocated memory for client info
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
    memset(&server_addr, 0, sizeof(server_addr));
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
        clientInfo->clientUniqueId = ++globalClientId;
        pthread_mutex_unlock(&idMutex);

        std::cout << "Assigned Client ID: " << clientInfo->clientUniqueId << std::endl;

        // Create a thread for the client
        pthread_t client_thread;
        if (pthread_create(&client_thread, nullptr, handleClientCommunication, (void *)clientInfo) != 0) {
            std::cerr << "Failed to create thread for client" << std::endl;
            free(clientInfo);
        } else {
            pthread_detach(client_thread);
        }
    }

    // Close the server socket (never reached if the loop runs indefinitely)
    close(server_socket);
    return 0;
}
