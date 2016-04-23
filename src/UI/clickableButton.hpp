// clickableButton.hpp
// a button which you click and things happen
#pragma once

#include "button.hpp"

class clickableButton : public button
    {
        private:
            std::function<void()> _onButtonPress;
            sf::Text _text;
            sf::Color _textColour;

        public:
            clickableButton();

            void setString(const std::string &buttonText);
            void setColour(sf::Color colour);
            void setSize(unsigned int size);
            void setPosition(sf::Vector2f pos);

            void setFunction(std::function<void()> onButtonPress);

            sf::Text *getText();
            const std::string getString();

            void render(sf::RenderWindow &app);
            void update();

    };