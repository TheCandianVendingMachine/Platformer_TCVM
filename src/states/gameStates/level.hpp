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
#include "../managers/events/observer.hpp"

#include "../game/globals.hpp"

#include "quadtree.hpp"

class gameObject;

class level : public observer
    {
        private:
            gameObjectFactory _factory;
            gameObject *_player;

            quadtree _quadtree;

            sf::Vector2f _levelSize;

            std::vector<gameObject*> _platforms;
            std::vector<gameObject*> _entities;

            std::string _currentLevel;
            bool _reloadLevel;

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
            quadtree *getQuadTree();

            void removeEntity(gameObject *obj);

            void alert(eventData data);

            ~level();

    };
