// level.hpp
// the game level. Holds all platforms and handles the loading of levels
#pragma once

#include <vector>
#include <memory>

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "../gameObject/gameObject/gameObjectFactory.hpp"

#include "../utilities/json/json/json.h"

#include "../game/globals.hpp"

class gameObject;

class level
    {
        private:
            gameObjectFactory _factory;

            std::vector<gameObject*> _platforms;
            std::vector<gameObject*> _entities;

        public:
            level();

            void unloadLevel();
            void load(const std::string &levelPath);
            void save(const std::string &levelPath);

            void update(sf::Time deltaTime);
            void draw(sf::RenderWindow &app);

            gameObject *addEntity(const std::string &name);
            gameObject *getEntityAtPosition(sf::Vector2f pos);

            std::unordered_map<std::string, std::vector<gameObject*>> *getAllGameObjects();
            std::unordered_map<std::string, std::vector<std::string>> *getInitializedObjects();

			size_t getAmountOfGameObjectsOnLevel();

            void removeEntity(gameObject *obj);

            ~level();

    };
