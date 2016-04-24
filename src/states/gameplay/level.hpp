// level.hpp
// the game level. Holds all platforms and handles the loading of levels
#pragma once

#include <vector>
#include <memory>

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "../../gameObject/gameObject/gameObjectFactory.hpp"

class gameObject;

class level
    {
        private:
            gameObjectFactory _factory;

            std::vector<gameObject*> _platforms;
            std::vector<gameObject*> _entities;

            std::string _currentEntityList;

        public:
            level();

            void load(const std::string &levelPath);
            void save(const std::string &levelPath);

            void update(sf::Time deltaTime);
            void draw(sf::RenderWindow &app);

            gameObject *addEntity(const std::string &name);

    };
