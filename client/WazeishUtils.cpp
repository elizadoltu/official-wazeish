#include "WazeishUtils.h"
#include "optionsBeginning.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <cmath>
#include <json-c/json.h>
#include <fstream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "../private/isStringDouble.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

bool clientSelectCity = false; 
bool clientSelectReports = false; 
bool accidentsReportsOption = false; 
bool eventReportsOption = false;
bool weatherReportsOption = false;
bool gasStationsOptions = false;
std::string selectedCity;

sf::RenderWindow mainWindow(sf::VideoMode(WIDTH, HEIGHT), "Wazeish");

void takeClientData(bool response, int clientSocket, int clientId, struct sockaddr_in serverAddr) {

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    bool responseData = false;

    if(response) {
        
        showWindowOptions();
        sf::Text inputText;
        inputText.setFont(font);
        inputText.setCharacterSize(24);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(300, 800);

        sf::RectangleShape inputBox;
        inputBox.setSize(sf::Vector2f(400.f, 30.f));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(2.f);
        inputBox.setPosition(300, 800);

        std::string inputSpeed;
        double speed;
        bool clientIntroducedSpeed = false;
        
        // WAIT FOR THE CLIENT TO COLLECT ALL THE DATA/OPTIONS
        while (!responseData) {

            sf::Event event;
            while (mainWindow.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    send(clientSocket, "Disconnected", strlen("Disconnected"), 0);
                    mainWindow.close();
                }

                if(event.type == sf::Event::MouseButtonPressed &&
                   event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(mainWindow);

                    // CITIES
                    sf::FloatRect iasiButtonBounds = iasiButton.getGlobalBounds();
                    sf::FloatRect bucurestiButtonBounds = bucurestiButton.getGlobalBounds();
                    sf::FloatRect timisoaraButtonBounds = timisoaraButton.getGlobalBounds();
                    sf::FloatRect targuOcnaButtonBounds = targuOcnaButton.getGlobalBounds();
                    sf::FloatRect clujNapocaButtonBounds = clujNapocaButton.getGlobalBounds();
                    sf::FloatRect brasovButtonBounds = brasovButton.getGlobalBounds();

                    // OPTIONS
                    sf::FloatRect accidentReportButtonBounds = accidentReportButton.getGlobalBounds();
                    sf::FloatRect eventsReportButtonBounds = eventsReportButton.getGlobalBounds();
                    sf::FloatRect weatherReportButtonBounds = weatherReportButton.getGlobalBounds();
                    sf::FloatRect gasStationButtonBounds = gasStationButton.getGlobalBounds();

                    // DESELECT BUTTON
                    sf::FloatRect deSelectButtonBounds = deSelectButton.getGlobalBounds();

                    // SEND DATA BUTTON
                    sf::FloatRect sendDataButtonBounds = sendDataButton.getGlobalBounds();

                    // THE CLIENT IS GOING ONLY TO SELECT ONE CITY
                    // THE VARIABLES WILL UPDATE THEIR VALUE DEPENDING WHERE THE CLIENT IS GOING TO CLICK

                    // IASI
                    if (iasiButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        iasiButton.setFillColor(sf::Color(28, 38, 54));
                        bucurestiButton.setFillColor(sf::Color(50, 67, 95));
                        timisoaraButton.setFillColor(sf::Color(50, 67, 95));
                        clujNapocaButton.setFillColor(sf::Color(50, 67, 95));
                        targuOcnaButton.setFillColor(sf::Color(50, 67, 95));
                        brasovButton.setFillColor(sf::Color(50, 67, 95));

                        selectedCity = "Iasi";
                        clientSelectCity = true;
                    }

                    // BUCURESTI
                    if (bucurestiButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        bucurestiButton.setFillColor(sf::Color(28, 38, 54));
                        iasiButton.setFillColor(sf::Color(50, 67, 95));
                        timisoaraButton.setFillColor(sf::Color(50, 67, 95));
                        clujNapocaButton.setFillColor(sf::Color(50, 67, 95));
                        targuOcnaButton.setFillColor(sf::Color(50, 67, 95));
                        brasovButton.setFillColor(sf::Color(50, 67, 95));

                        selectedCity = "Bucuresti";
                        clientSelectCity = true;
                    }

                    // TIMISOARA 
                    if (timisoaraButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        timisoaraButton.setFillColor(sf::Color(28, 38, 54));
                        iasiButton.setFillColor(sf::Color(50, 67, 95));
                        bucurestiButton.setFillColor(sf::Color(50, 67, 95));
                        clujNapocaButton.setFillColor(sf::Color(50, 67, 95));
                        targuOcnaButton.setFillColor(sf::Color(50, 67, 95));
                        brasovButton.setFillColor(sf::Color(50, 67, 95));

                        selectedCity = "Timisoara";
                        clientSelectCity = true;
                    }

                    // CLUJ-NAPOCA
                    if (clujNapocaButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        clujNapocaButton.setFillColor(sf::Color(28, 38, 54));
                        iasiButton.setFillColor(sf::Color(50, 67, 95));
                        timisoaraButton.setFillColor(sf::Color(50, 67, 95));
                        bucurestiButton.setFillColor(sf::Color(50, 67, 95));
                        targuOcnaButton.setFillColor(sf::Color(50, 67, 95));
                        brasovButton.setFillColor(sf::Color(50, 67, 95));

                        selectedCity = "Cluj-Napoca";
                        clientSelectCity = true;
                    }

                    // TARGU OCNA
                    if (targuOcnaButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        targuOcnaButton.setFillColor(sf::Color(28, 38, 54));
                        iasiButton.setFillColor(sf::Color(50, 67, 95));
                        timisoaraButton.setFillColor(sf::Color(50, 67, 95));
                        clujNapocaButton.setFillColor(sf::Color(50, 67, 95));
                        bucurestiButton.setFillColor(sf::Color(50, 67, 95));
                        brasovButton.setFillColor(sf::Color(50, 67, 95));

                        selectedCity = "Targu Ocna";
                        clientSelectCity = true;
                    }

                    // BRASOV
                    if (brasovButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        brasovButton.setFillColor(sf::Color(28, 38, 54));
                        iasiButton.setFillColor(sf::Color(50, 67, 95));
                        timisoaraButton.setFillColor(sf::Color(50, 67, 95));
                        clujNapocaButton.setFillColor(sf::Color(50, 67, 95));
                        targuOcnaButton.setFillColor(sf::Color(50, 67, 95));
                        bucurestiButton.setFillColor(sf::Color(50, 67, 95));

                        selectedCity = "Brasov";

                        clientSelectCity = true;
                    }

                    // ACCIDENTS
                    if (accidentReportButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        accidentReportButton.setFillColor(sf::Color(68, 82, 78));
                        accidentsReportsOption = true;
                        clientSelectReports = true;
                    }

                    // EVENTS
                    if (eventsReportButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))){
                        eventsReportButton.setFillColor(sf::Color(68, 82, 78));
                        eventReportsOption = true;
                        clientSelectReports = true;
                    }

                    // WEATHER
                    if (weatherReportButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))){
                        weatherReportsOption = true;
                        clientSelectReports = true;
                        weatherReportButton.setFillColor(sf::Color(68, 82, 78));
                    }

                    // GAS STATION PRICES
                    if (gasStationButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        gasStationButton.setFillColor(sf::Color(68, 82, 78));
                        gasStationsOptions = true;
                        clientSelectReports = true;
                    }

                    // DESELECT BUTTON
                    if (deSelectButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        
                        // IT RESETS EVERYTHING 
                        deSelectButton.setFillColor(sf::Color(46, 46, 46));

                        iasiButton.setFillColor(sf::Color(50, 67, 95));
                        bucurestiButton.setFillColor(sf::Color(50, 67, 95));
                        timisoaraButton.setFillColor(sf::Color(50, 67, 95));
                        clujNapocaButton.setFillColor(sf::Color(50, 67, 95));
                        targuOcnaButton.setFillColor(sf::Color(50, 67, 95));
                        brasovButton.setFillColor(sf::Color(50, 67, 95));

                        accidentReportButton.setFillColor(sf::Color(105, 128, 122));
                        eventsReportButton.setFillColor(sf::Color(105, 128, 122));
                        weatherReportButton.setFillColor(sf::Color(105, 128, 122));
                        gasStationButton.setFillColor(sf::Color(105, 128, 122));


                        clientSelectCity = false; 
                        clientSelectReports = false; 
                        accidentsReportsOption = false; 
                        eventReportsOption = false;
                        weatherReportsOption = false;
                        gasStationsOptions = false;
                        selectedCity = "";

                        deSelectButton.setFillColor(sf::Color::Black);
                    }
                    if (clientSelectCity && clientSelectReports && clientIntroducedSpeed) {
                        if (sendDataButtonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)) && clientIntroducedSpeed) {
                            responseData = true;
                            sendClientData(clientId, clientSocket, selectedCity, speed, accidentsReportsOption, eventReportsOption, gasStationsOptions, weatherReportsOption);
                            gasStationsOptions = false;
                            accidentsReportsOption = false;
                            eventReportsOption = false;
                            weatherReportsOption = false;

                            clientSelectCity = false;
                            clientSelectReports = false;
                        }
                    }
                }
                if (event.type == sf::Event::TextEntered) {
                    if(event.text.unicode < 128) {
                       char inputChar = static_cast<char>(event.text.unicode);

                        if (event.text.unicode == 8) {
                            // BACKSPACE
                            if(!inputSpeed.empty()) {
                               inputSpeed.pop_back();
                            }
                        } else if(event.text.unicode == 13 ) {
                            // ENTER (IF THE CLIENT SENDS IT)
                            std::cout << "[client]Entered: " << inputSpeed << '\n';

                            if(isDouble(inputSpeed)) {
                                speed = std::stod(inputSpeed);
                                clientIntroducedSpeed = true;
                            } else if (!isDouble(inputSpeed)) {
                                std::cout << "[client]Try again! Invalid input." << '\n';
                                inputSpeed.clear();
                            }
                        } else {
                            inputSpeed += inputChar;
                    }
                    inputText.setString(inputSpeed);
                    }
                }
            }

            mainWindow.clear(sf::Color(194, 200, 198));
            mainWindow.draw(iasiButton);
            mainWindow.draw(iasiText);

            mainWindow.draw(targuOcnaButton);
            mainWindow.draw(targuOcnaText);

            mainWindow.draw(bucurestiButton);
            mainWindow.draw(bucurestiText);

            mainWindow.draw(clujNapocaButton);
            mainWindow.draw(clujNapocaText);

            mainWindow.draw(timisoaraButton);
            mainWindow.draw(timisoaraText);

            mainWindow.draw(brasovButton);
            mainWindow.draw(brasovText);

            mainWindow.draw(accidentReportButton);
            mainWindow.draw(accidentsText);

            mainWindow.draw(eventsReportButton);
            mainWindow.draw(eventsText);

            mainWindow.draw(weatherReportButton);
            mainWindow.draw(weatherText);

            mainWindow.draw(gasStationButton);
            mainWindow.draw(gasPricesText);

            mainWindow.draw(deSelectButton);
            mainWindow.draw(deSelectText);

            mainWindow.draw(sendDataButton);
            mainWindow.draw(sendDataText);

            mainWindow.draw(inputBox);
            mainWindow.draw(inputText);

            mainWindow.display();

        }
    }
    close(clientSocket);
}

bool doCircleIntersect(const sf::Vector2f &pos1, const sf::Vector2f &pos2, float radius) {

    float distance = std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
    return distance < 2 * radius; 
    // CIRCLE INTERSECT IF THE DISTANCE NETWEEN THEM IS LESS THAN TWICE THE RADIUS
}

std::vector<std::pair<sf::Vector2f, std::string>> loadStreetInfoFromJson(const char* filename, const std::string& cityName) {
    std::vector<std::pair<sf::Vector2f, std::string>> streetInfo;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open JSON file: " << filename << '\n';
        return streetInfo;
    }

    // READ THE FILE CONTENT INTO A STRING
    std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // PARSE THE JSON STRING
    json_object* jsonObj = json_tokener_parse(jsonString.c_str());

    // GET THE "cities" ARRAY
    json_object* citiesArray;
    if (json_object_object_get_ex(jsonObj, "cities", &citiesArray) &&
        json_type_array == json_object_get_type(citiesArray)) {

        int arrayLength = json_object_array_length(citiesArray);
        for (int index = 0; index < arrayLength; ++index) {
            json_object* cityObj = json_object_array_get_idx(citiesArray, index);

            // CHECK IF THE CURRENT CITY MATCHES THE DESIRED CITY
            json_object* cityNameObj;
            if (json_object_object_get_ex(cityObj, "city_name", &cityNameObj) &&
                json_type_string == json_object_get_type(cityNameObj) &&
                cityName == json_object_get_string(cityNameObj)) {

                // EXTRACT STREETS INFORMATION FOR THE DESIRED CITY
                json_object* streetArray;
                if (json_object_object_get_ex(cityObj, "streets", &streetArray) &&
                    json_type_array == json_object_get_type(streetArray)) {

                    int streetArrayLength = json_object_array_length(streetArray);
                    for (int j = 0; j < streetArrayLength; ++j) {
                        json_object* streetObj = json_object_array_get_idx(streetArray, j);

                        // EXTRACT THE INFO
                        json_object* streetNameObj;
                        json_object* streetNumberObj;
                        json_object* streetColorObj;

                        if (json_object_object_get_ex(streetObj, "street_name", &streetNameObj) &&
                            json_object_object_get_ex(streetObj, "number", &streetNumberObj) &&
                            json_object_object_get_ex(streetObj, "street_color", &streetColorObj) &&
                            json_type_string == json_object_get_type(streetNameObj) &&
                            json_type_int == json_object_get_type(streetNumberObj) &&
                            json_type_string == json_object_get_type(streetColorObj)) {

                            // CREATE A PAIR WITH STREET INFORMATION
                            sf::Vector2f coordinates;
                            coordinates.x = static_cast<float>(rand() % 780 + 50);
                            coordinates.y = static_cast<float>(rand() % 780 + 50);

                            streetInfo.emplace_back(coordinates, json_object_get_string(streetNameObj));
                        }
                    }
                }
                break;
            }
        }
    } else {
        std::cerr << "Failed to find the 'cities' array in the JSON file" << '\n';
    }

    // CLEAN UP
    json_object_put(jsonObj);

    return streetInfo;
}



/*
void renderImages(int clientSocket, int clientId, struct sockaddr_in serverAddr) {
    bool verifyCity = false;

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    std::string clientIdString = std::to_string(clientId);
    const char* clientIdChar = clientIdString.c_str();
    send(clientSocket, clientIdChar, strlen(clientIdChar), 0);
    char receivedCity[1024];
    int bytes_received = recv(clientSocket, receivedCity, sizeof(receivedCity) - 1, 0);

    if (bytes_received == -1) {
        // Handle receive error
        perror("Error receiving data from server 1");
    } else if (bytes_received == 0) {
        // Connection closed by the server
        std::cerr << "Connection closed by the server" << std::endl;
    } else {
        // Data received successfully
        receivedCity[bytes_received] = '\0';  // Null-terminate the received data
        std::cout << "Received city: " << receivedCity << std::endl;
    }

    if (bytes_received > 0) {
        while (!verifyCity) {
            std::cout << "[client]Start" << '\n';
            if (strcmp(receivedCity, "Iasi") == 0 || strcmp(receivedCity, "Targu Ocna") == 0 ||
                strcmp(receivedCity, "Brasov") == 0 || strcmp(receivedCity, "Bucuresti") == 0 ||
                strcmp(receivedCity, "Cluj-Napoca") == 0 || strcmp(receivedCity, "Timisoara") == 0) {

                // CITY IS VALID
                verifyCity = true;

                std::cout << "Received city: " << receivedCity << std::endl;

                // VECTOR TO STORE POINTS AND THEIR NAMES
                std::vector<std::pair<sf::Vector2f, std::string>> streetInfo;

                if (!font.loadFromFile("../fonts/arial.ttf")) {
                    std::cerr << "Error loading font file" << std::endl;
                    return;
                }

                // LOAD STREET INFORMATION FOR THE DESIRED CITY
                streetInfo = loadStreetInfoFromJson("../json/cities.json", receivedCity);

                while (mainWindow.isOpen()) {
                    sf::Event event;
                    while (mainWindow.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            mainWindow.close();
                        }
                    }

                    mainWindow.clear(sf::Color::White);

                    // DRAW POINTS AND COLLECTING LINES
                    for (size_t i = 0; i < streetInfo.size(); ++i) {
                        // DRAW POINT
                        sf::CircleShape point(5);
                        point.setFillColor(sf::Color::Green);
                        point.setPosition(streetInfo[i].first);
                        mainWindow.draw(point);

                        // DRAW STREET NAME
                        sf::Text text(streetInfo[i].second, font, 12);
                        text.setPosition(streetInfo[i].first.x, streetInfo[i].first.y - 20);
                        mainWindow.draw(text);

                        // DRAW CONNECTING LINES
                        for (size_t j = i + 1; j < streetInfo.size(); ++j) {
                            sf::VertexArray line(sf::Lines, 2);
                            line[0].position = streetInfo[i].first;
                            line[1].position = streetInfo[j].first;
                            line[0].color = sf::Color::Green;
                            line[1].color = sf::Color::Green;
                            mainWindow.draw(line);
                        }
                    }

                    mainWindow.display();
                }
            } else {
                // CITY IS NOT VALID
                std::cerr << "Invalid city received: " << receivedCity << std::endl;

                // EXIT THE LOOP
                verifyCity = true;
            }
        }
    }

    // Move close(clientSocket) outside the loop
    close(clientSocket);
}
*/

void renderImages(int clientSocket, int clientId, struct sockaddr_in serverAddr) {
    std::cout << "[client] Start" << '\n';
    sf::Clock clock;

    // Always treat the city as Iasi
    const char* receivedCity = "Iasi";
    std::cout << "Received city: " << receivedCity << std::endl;

    // VECTOR TO STORE POINTS AND THEIR NAMES
    std::vector<std::pair<sf::Vector2f, std::string>> streetInfo;
    bool wasDrawnLine = false;
    bool cityNameDrawn = false;
    if (!font.loadFromFile("../fonts/arial.ttf")) {
        std::cerr << "Error loading font file" << std::endl;
        close(clientSocket);
        return;
    }

    // LOAD STREET INFORMATION FOR THE DESIRED CITY
    streetInfo = loadStreetInfoFromJson("../json/cities.json", receivedCity);
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> randomLines;
    for (size_t i = 0; i < streetInfo.size(); ++i) {
        size_t j = rand() % streetInfo.size();
        if (i != j) {
            randomLines.emplace_back(streetInfo[i].first, streetInfo[j].first);
        }
    }
    
    if (streetInfo.empty()) {
        std::cerr << "Error loading street information" << std::endl;
        close(clientSocket);
        return;
    }


    mainWindow.clear(sf::Color(240, 240, 240));

    while (mainWindow.isOpen()) {
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mainWindow.close();
            }
        }
        mainWindow.clear(sf::Color(240, 240, 240));
        // DRAW POINTS
        for (const auto& street : streetInfo) {
            // DRAW POINT
            sf::CircleShape point(8);  // Increase point size
            point.setFillColor(sf::Color(46, 134, 193));  // Blue color
            point.setPosition(street.first);
            mainWindow.draw(point);

            // DRAW STREET NAME
            sf::Text text(street.second, font, 14);  // Increase font size
            text.setFillColor(sf::Color::Black);  // Set text color to black
            // Adjust the position to make sure it's visible
            text.setPosition(street.first.x + 10, street.first.y - 20);
            mainWindow.draw(text);
        }

        // DRAW RANDOM LINES
        for (const auto& line : randomLines) {
            // Draw the line
            sf::VertexArray randomLine(sf::Lines, 2);
            randomLine[0].position = line.first;
            randomLine[1].position = line.second;
            randomLine[0].color = sf::Color(46, 134, 193);  // Blue color
            randomLine[1].color = sf::Color(46, 134, 193);  // Blue color
            mainWindow.draw(randomLine);

            // Draw a circle at each endpoint of the line to prevent crossing
            sf::CircleShape endpoint(8);  // Increase circle size
            endpoint.setFillColor(sf::Color(46, 134, 193));  // Blue color
            endpoint.setPosition(line.first);
            mainWindow.draw(endpoint);

            endpoint.setPosition(line.second);
            mainWindow.draw(endpoint);
        }

        sf::Text mapText("Iasi Map", font, 28);  // Increase font size
        mapText.setFillColor(sf::Color::Black);
        mapText.setPosition(10, 10);  // Adjust the position
        mainWindow.draw(mapText);
        mainWindow.display();
    }

    close(clientSocket);
}
