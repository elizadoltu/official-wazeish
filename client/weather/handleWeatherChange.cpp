#include <iostream>
#include <sys/socket.h>
#include <string.h>
void reportWeatherChange(int client_socket) {
    std::cout << "\n=== Reporting Weather Change ===\n";

    std::string weather;
    std::cout << "Enter the current weather\n";
    std::cout << "1. Sunny\n";
    std::cout << "2. Rainy\n";
    std::cout << "3. Snowy\n";
    std::cout << "4. Windy\n";
    std::cout << "Type here: ";
    std::cin >> weather;
    std::cout << "Debug: Sending weather: " << weather << std::endl;

    // Send the weather condition
    write(client_socket, weather.c_str(), weather.length());

    std::cout << "Server response: \n";

        double speed;
        std::cout << "Enter your current speed: ";
        std::cin >> speed;

        std::string speedStr = std::to_string(speed);
        std::cout << "Debug: Sending speed: " << speedStr << std::endl;
        write(client_socket, speedStr.c_str(), speedStr.length());

}
