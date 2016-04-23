// level.hpp
// the game level. Holds all platforms and handles the loading of levels
#pragma once

#include <vector>
#include <memory>

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class entity;

class level
    {
        private:
            std::vector<std::shared_ptr<entity>> _platforms;

        public:
            void draw(sf::RenderWindow &app);
            void handleCollision(entity *collider);

            void load(const std::string &levelPath);
            void save(const std::string &levelPath);

            entity *addPlatform();
            void removePlatform(entity *platform);

            entity *getPlatformAt(sf::Vector2f pos);

    };