// gameObjectFactory.hpp
// the factory for all game objects. Initializes, returns, and holds all game objects.
// has its own texture manager to hold all current textures. Initializes a Json file of
// all entities wanted, and puts them into pre-fab entities
#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "../../managers/resourceManager.hpp"
#include "../../managers/events/observer.hpp"

#include "../../utilities/json/json/json.h"
#include <SFML/Graphics/Texture.hpp>

class gameObject;

class gameObjectFactory : public observer
    {
        private:
            std::unordered_map<std::string, std::vector<gameObject*>> _gameObjects;
            // all game objects that have been initialized.
            std::vector<gameObject*> _gameObjectPool;
            // file path, entity name
            std::unordered_map<std::string, std::vector<std::string>> _initializedFiles;

            resourceManager<sf::Texture> _textureManager;

            gameObject *fetchFromPool(const std::string &name);

        public:
            gameObjectFactory();
            
            gameObject *addGameObject(const std::string &objectName);
            void removeGameObject(gameObject *obj);

            void initializeJsonFile(const std::string &filepath);
            void deInitializeJsonFile(const std::string &filepath);
            void deInitializeJsonFile();

            void clear();

            std::vector<gameObject*> *getGameObjects(const std::string &objectName);

            std::unordered_map<std::string, std::vector<gameObject*>> *getGameObjects();
            std::unordered_map<std::string, std::vector<std::string>> *getInitializedEntities();

            void alert(eventData _data);

            ~gameObjectFactory();

    };