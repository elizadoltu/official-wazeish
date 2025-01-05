#include "optionsBeginning.h"
#include <iostream>
#include <SFML/Graphics.hpp>

sf::Font font;

sf::RectangleShape iasiButton(sf::Vector2f(250, 70));
sf::Text iasiText("Iasi", font, 25);

sf::RectangleShape targuOcnaButton(sf::Vector2f(250, 70));
sf::Text targuOcnaText("Targu Ocna", font, 25);

sf::RectangleShape clujNapocaButton(sf::Vector2f(250, 70));
sf::Text clujNapocaText("Cluj-Napoca", font, 25);

sf::RectangleShape timisoaraButton(sf::Vector2f(250, 70));
sf::Text timisoaraText("Timisoara", font, 25);

sf::RectangleShape brasovButton(sf::Vector2f(250, 70));
sf::Text brasovText("Brasov", font, 25);

sf::RectangleShape bucurestiButton(sf::Vector2f(250, 70));
sf::Text bucurestiText("Bucuresti", font, 25);

sf::RectangleShape accidentReportButton(sf::Vector2f(250, 70));
sf::Text accidentsText("Accidents", font, 25);

sf::RectangleShape eventsReportButton(sf::Vector2f(250, 70));
sf::Text eventsText("Events", font, 25);

sf::RectangleShape weatherReportButton(sf::Vector2f(250, 70));
sf::Text weatherText("Weather", font, 25);

sf::RectangleShape gasStationButton(sf::Vector2f(250, 70));
sf::Text gasPricesText("Gas Prices", font, 25);

sf::RectangleShape deSelectButton(sf::Vector2f(250, 70));
sf::Text deSelectText("Deselect", font, 25);

sf::RectangleShape sendDataButton(sf::Vector2f(250, 70));
sf::Text sendDataText("Send", font, 25);

void showWindowOptions() {
    if (!font.loadFromFile("../fonts/arial.ttf")) {
        std::cerr << "Error loading font file" << std::endl;
    }

    // Calculate center position for columns along x-axis
    float column1X = (1000 - 250) / 4;
    float column2X = (1000 - 250) * 3 / 4;

    // Calculate center position for buttons along y-axis
    float centerY = (1000 - 7 * 70) / 2;

    // Cities Buttons
    iasiButton.setPosition(column1X, centerY);
    iasiButton.setFillColor(sf::Color(50, 67, 95));
    iasiText.setPosition(column1X + 25, centerY + 15);
    iasiText.setFillColor(sf::Color::White);

    targuOcnaButton.setPosition(column1X, centerY + 80);
    targuOcnaButton.setFillColor(sf::Color(50, 67, 95));
    targuOcnaText.setPosition(column1X + 25, centerY + 95);
    targuOcnaText.setFillColor(sf::Color::White);

    clujNapocaButton.setPosition(column1X, centerY + 160);
    clujNapocaButton.setFillColor(sf::Color(50, 67, 95));
    clujNapocaText.setPosition(column1X + 25, centerY + 175);
    clujNapocaText.setFillColor(sf::Color::White);

    timisoaraButton.setPosition(column1X, centerY + 240);
    timisoaraButton.setFillColor(sf::Color(50, 67, 95));
    timisoaraText.setPosition(column1X + 25, centerY + 255);
    timisoaraText.setFillColor(sf::Color::White);

    brasovButton.setPosition(column1X, centerY + 320);
    brasovButton.setFillColor(sf::Color(50, 67, 95));
    brasovText.setPosition(column1X + 25, centerY + 335);
    brasovText.setFillColor(sf::Color::White);

    bucurestiButton.setPosition(column1X, centerY + 400);
    bucurestiButton.setFillColor(sf::Color(50, 67, 95));
    bucurestiText.setPosition(column1X + 25, centerY + 415);
    bucurestiText.setFillColor(sf::Color::White);

    // Reports Buttons
    accidentReportButton.setPosition(column2X, centerY);
    accidentReportButton.setFillColor(sf::Color(105, 128, 122));
    accidentsText.setPosition(column2X + 25, centerY + 15);
    accidentsText.setFillColor(sf::Color::White);

    eventsReportButton.setPosition(column2X, centerY + 80);
    eventsReportButton.setFillColor(sf::Color(105, 128, 122));
    eventsText.setPosition(column2X + 25, centerY + 95);
    eventsText.setFillColor(sf::Color::White);

    weatherReportButton.setPosition(column2X, centerY + 160);
    weatherReportButton.setFillColor(sf::Color(105, 128, 122));
    weatherText.setPosition(column2X + 25, centerY + 175);
    weatherText.setFillColor(sf::Color::White);

    gasStationButton.setPosition(column2X, centerY + 240);
    gasStationButton.setFillColor(sf::Color(105, 128, 122));
    gasPricesText.setPosition(column2X + 25, centerY + 255);
    gasPricesText.setFillColor(sf::Color::White);

    // Control Buttons
    deSelectButton.setPosition(column2X, centerY + 320);
    deSelectButton.setFillColor(sf::Color::Black);
    deSelectText.setPosition(column2X + 25, centerY + 335);
    deSelectText.setFillColor(sf::Color::White);

    sendDataButton.setPosition(column2X, centerY + 400);
    sendDataButton.setFillColor(sf::Color::Black);
    sendDataText.setPosition(column2X + 25, centerY + 415);
    sendDataText.setFillColor(sf::Color::White);
}
