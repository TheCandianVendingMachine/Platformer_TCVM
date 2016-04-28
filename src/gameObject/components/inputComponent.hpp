// inputComponent.hpp
// allows the character to move
#pragma once

#include "component.hpp"

#include <SFML/Window/Keyboard.hpp> 
#include <string>

namespace Json
    {
        class Value;
    }

class inputComponent : public component
    {
        private:
            void loadJsonFile(const std::string &file, Json::Value *root);

        public:
            sf::Keyboard::Key setKey(const std::string &key);

    };