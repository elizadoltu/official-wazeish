#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h>
#include <algorithm>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <thread>  // For threading

#include "options/saveClientData.cpp"
#include "weather/handleWeatherChange.cpp"
#include "accidents/handleAccidentsReports.cpp"
#include "events/handleEvents.cpp"

#define PORT 1168
#define SERVER_IP "127.0.0.1"

void reportGasPriceChange(int client_socket) {
    std::cout << "\n=== Reporting Gas Price Change ===\n";
    std::string stationName;
    double gasolinePrice, dieselPrice;

    // Get the gas station name and prices from the user
    std::cout << "Enter the name of the gas station: ";
    std::cin >> stationName;

    std::cout << "Enter the price for gasoline: ";
    std::cin >> gasolinePrice;

    std::cout << "Enter the price for diesel: ";
    std::cin >> dieselPrice;

    // Send the data to the server in comma-separated format
    std::string report = stationName + "," + std::to_string(gasolinePrice) + "," + std::to_string(dieselPrice);
    write(client_socket, report.c_str(), report.length()); // Changed from send to write
}

int generateRandomClientId() {
    // Generate a random client ID between 1000 and 9999
    return 1000 + (std::rand() % 9000);
}

bool keepListening = true;

void listenForServerMessages(int client_socket) {
    char responseBuffer[1024];
    while (keepListening) {
        ssize_t bytes_received = read(client_socket, responseBuffer, sizeof(responseBuffer) - 1); // Changed from recv to read
        if (bytes_received > 0) {
            responseBuffer[bytes_received] = '\0'; // Null-terminate the response
            std::cout << "\n[Broadcast Message]: " << responseBuffer << std::endl;
        } else if (bytes_received == 0) {
            std::cout << "\nServer closed the connection.\n";
            break;
        } else {
            if (keepListening) { // Only show error if not stopping intentionally
                std::cerr << "\nError receiving data from server!\n";
            }
            break;
        }
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        return -1;
    }

    std::cout << "Connected to server.\n";

    // Start a separate thread to listen for broadcast messages
    std::thread listener_thread(listenForServerMessages, client_socket);

    while (true) {
        int clientId = generateRandomClientId();
        std::cout << "\nGenerated Client ID: " << clientId << "\n";

        std::cout << "\n=== User Registration ===\n";
        std::string city;
        double speed;
        std::string newsPreferences;

        // Collect user data
        std::cout << "Enter your speed (e.g., 45.0): ";
        std::cin >> speed;

        std::cout << "Select the news you want to receive (1: Accident, 2: Weather, 3: Events, 4: Gas prices).\n";
        std::cout << "Enter your choices as comma-separated values (e.g., 1,2,4): ";
        std::cin.ignore();
        std::getline(std::cin, newsPreferences);

        // Parse the user's choices
        std::vector<int> selectedOptions = parseOptions(newsPreferences);

        // Determine report choices
        bool chooseAccidentsReports = std::find(selectedOptions.begin(), selectedOptions.end(), 1) != selectedOptions.end();
        bool chooseWeatherReports = std::find(selectedOptions.begin(), selectedOptions.end(), 2) != selectedOptions.end();
        bool chooseEventReports = std::find(selectedOptions.begin(), selectedOptions.end(), 3) != selectedOptions.end();
        bool chooseGasStationsReports = std::find(selectedOptions.begin(), selectedOptions.end(), 4) != selectedOptions.end();

        // Send data to server
        sendClientData(clientId, client_socket, speed, chooseAccidentsReports, chooseEventReports,
                       chooseGasStationsReports, chooseWeatherReports);

        // Reporting menu
        while (true) {
            char responseBuffer[1024] = {0};
            std::cout << "\n=== Reporting Menu ===\n";
            std::cout << "What would you like to report?\n";
            std::cout << "1. Accident\n";
            std::cout << "2. Weather change\n";
            std::cout << "3. Event\n";
            std::cout << "4. Gas price change\n";
            std::cout << "5. Exit reporting menu\n";
            std::cout << "Enter your choice: ";

            int reportChoice;
            std::cin >> reportChoice;

            if (reportChoice == 1) {
                reportAccidents(client_socket);
            } else if (reportChoice == 2) {
                reportWeatherChange(client_socket);
            } else if (reportChoice == 3) {
                reportEvent(client_socket);
            } else if (reportChoice == 4) {
                reportGasPriceChange(client_socket);
            } else if (reportChoice == 5) {
                std::cout << "Exiting reporting menu...\n";
                break;
            } else {
                std::cout << "Invalid option. Please try again.\n";
            }
        }

        std::cout << "Would you like to register another user? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    // Signal the listener thread to stop
    keepListening = false;

    // Close the socket
    close(client_socket);

    // Wait for the listener thread to finish
    listener_thread.join();

    return 0;
}
