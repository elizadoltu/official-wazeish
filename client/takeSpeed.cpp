#include "WazeishUtils.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string.h>
#include <sstream>

#include "../private/isStringDouble.h"

double takeClientSpeed() {

    if(!font.loadFromFile("../fonts/arial.ttf")){
        std::cerr << "Error loading font file" << '\n';
    }

    bool responseReady = false;

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(300, 350);

    sf::RectangleShape inputBox;
    inputBox.setSize(sf::Vector2f(400.f, 30.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2.f);
    inputBox.setPosition(250, 300);

    // INPUT BUFFER FOR USER MESSAGE
    std::string inputSpeed;

    while(!responseReady) {
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                mainWindow.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if(event.text.unicode < 128) {
                    char inputChar = static_cast<char>(event.text.unicode);

                    if (event.text.unicode == 8) {
                        // BACKSPACE
                        if(!inputSpeed.empty()) {
                            inputSpeed.pop_back();
                        }
                    } else if(event.text.unicode == 13) {
                        // ENTER (IF THE CLIENT SENDS IT)
                        std::cout << "[client]Entered: " << inputSpeed << '\n';

                        if(isDouble(inputSpeed)) {
                            responseReady = true;
                            break;
                        } else if (!isDouble(inputSpeed)) {
                            std::cout << "[client]Try again! Invalid input.";
                            inputSpeed.clear();
                        } else {
                            inputSpeed += inputChar;
                        }
                    }
                    inputText.setString(inputSpeed);
                }
            }
        }

        mainWindow.clear(sf::Color::White);
        mainWindow.draw(inputBox);
        mainWindow.draw(inputText);
        mainWindow.display();
    }

    return stod(inputSpeed);
}
