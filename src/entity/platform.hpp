// platform.hpp
// the platforms which the entities will collide with
#pragma once

#include "entity.hpp"
#include "../game/globals.hpp"

class platform : public entity
    {
        public:
            platform() { _entID = PLATFORM; setTexture(globals::_textureManager->get("platformTexture", false)); }
            void update(sf::Time deltaTime) {}
    };