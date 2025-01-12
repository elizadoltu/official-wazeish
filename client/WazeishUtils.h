#pragma once

#ifndef WazeishUtils_H
#define WazeishUtils_H

#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include <vector>
#define WIDTH 1000
#define HEIGHT 1000

extern bool clientSelectCity; 
extern bool clientSelectReports;
extern std::string selectedCity;
extern bool accidentsReportsOption;
extern bool eventReportsOption;
extern bool weatherReportsOption;
extern bool gasStationsOptions;

// GLOBAL VARIABLES
extern sf::RenderWindow mainWindow;

// CLIENT FILE
extern sf::Font font;

// BUILD AND RETURN A JSON FILE READY TO BE SEND 
extern void sendClientData(int clientId, int clientSocket, std::string city, double speed,bool chooseAccidentsReports, bool chooseEventReports, bool chooseGasStationsReports, bool chooseWeatherReports);

// SEPARATE FUNCTION TO TAKE THE CLIENT'S SPEED
extern double takeClientSpeed();

// RETURN THE OPTIONS THAT THE CLIENT SELECTED
extern void takeClientData(bool response, int clientSocket, int clientId, struct sockaddr_in serverAddr);

// RENDER THE LAYOUT ONLY
extern void showWindowOptions();

// RENDER THE IMAGES OF CITITES ONLY
extern void renderImages(int clientSocket, int clientId, struct sockaddr_in serverAddr);

// FUNCTION TO CHECK IF TWO CIRCLES (POINTS) INTERSECT
bool doCircleIntersect(const sf::Vector2f &pos1, const sf::Vector2f &pos2, float radius);

// FUNCTION TO LOAD STREET INFORMATION FROM A JSON FILE FOR A SPECIFIC CITY
std::vector<std::pair<sf::Vector2f, std::string>> loadStreetInfoFromJson(const char* filename, const std::string& cityName);

// STYLE ONLY
void drawCityButton(sf::RectangleShape& button, sf::Text& buttonText, const std::string& cityName);

// STYLE ONLY
void drawReportsButton(sf::RectangleShape& button, sf::Text& buttonText, const std::string& reportType);

// STYLE ONLY
void drawButton(sf::RectangleShape& button, sf::Text& buttonText, const std::string& text);
#endif
