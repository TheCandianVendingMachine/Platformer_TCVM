// userInterfaceFactory.hpp
// allows for easy managing/creating of UI's. Allows for bare-bone text, and various UI Gizmos
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../managers/resourceManager.hpp"

#include <unordered_map>
#include <string>
#include <memory>

class button;

class userInterfaceFactory
    {
        private:
            resourceManager<sf::Font> _fonts;

            std::unordered_map<std::string, button*> _UIButtons;
            std::unordered_map<std::string, sf::Text> _UIText;

        public:
            void setFont(const std::string &filepath);
            sf::Font *getFont();

            void add(const std::string &name, const std::string &text);
            void add(const std::string &name, button *obj);

            button *getButton(const std::string &name);
            sf::Text *getText(const std::string &name);

            void draw(sf::RenderWindow &app);
            void update();
        
            ~userInterfaceFactory();
    };