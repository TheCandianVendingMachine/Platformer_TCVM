// inputComponent.hpp
// allows the character to move
#pragma once

#include "component.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <string>

class inputComponent : public component
    {
        public:
            sf::Keyboard::Key setKey(const std::string &key);

    };