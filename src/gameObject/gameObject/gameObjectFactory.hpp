// gameObjectFactory.hpp
// the factory for all game objects. Initializes, returns, and holds all game objects.
// has its own texture manager to hold all current textures. Initializes a Json file of
// all entities wanted, and puts them into pre-fab entities
#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "../../managers/resourceManager.hpp"

#include <json/json.h>
#include <SFML/Graphics/Texture.hpp>

class gameObject;

class gameObjectFactory
    {
        private:
            std::unordered_map<std::string, std::vector<gameObject*>> _gameObjects;
            std::vector<std::string> _initializedFiles;

            resourceManager<sf::Texture> _textureManager;

            void loadJsonFile(const std::string &file, Json::Value *root);

        public:
            gameObject *addGameObject(const std::string &objectName);
            void removeGameObject(gameObject *obj);

            void initializeJsonFile(const std::string &filepath);
            void deInitializeJsonFile(const std::string &filepath);
            void deInitializeJsonFile();

            std::vector<gameObject*> *getGameObjects(const std::string &objectName);
            std::unordered_map<std::string, std::vector<gameObject*>> *getGameObjects();

            ~gameObjectFactory();

    };