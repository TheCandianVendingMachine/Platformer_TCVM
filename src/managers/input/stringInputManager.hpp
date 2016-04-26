// stringInputManager.hpp
// manages string input. A global class that reads data from sf::Event::TextEntered and allows anything to call the data processed
#pragma once

#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>

class stringInputManager
    {
        private:
            sf::String _textEntered;

        public:
            void clearString();
            void getInput(sf::Event &event);
            sf::String &getTextEntered();


    };