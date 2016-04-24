#include "gameObjectFactory.hpp"
#include "gameObject.hpp"

#include "../../game/globals.hpp"

#include "../components/components.hpp"

#include <json/json.h>
#include <fstream>

gameObject *gameObjectFactory::addGameObject(const std::string &objectName, const std::string &filepath)
    {
        globals::_logger->logToConsole("Initializing \"" + objectName + "\"");

        Json::Value root;

        std::ifstream read(filepath);
        if (read.is_open())
            {
                read >> root;
            }
        read.close();

        gameObject *newObj = new gameObject(objectName);
        for (auto &comp : root[objectName].getMemberNames())
            {
                globals::_logger->logToConsole("Adding \"" + comp + "\" to \"" + objectName + "\"");

                if (comp == "textureComponent")
                    {
                        textureComponent *tc = new textureComponent;
                        tc->setTexture(*_textureManager.get(objectName + "Texture", false));
                        tc->setGameObject(newObj);

                        newObj->addComponent(std::type_index(typeid(textureComponent)), tc);
                    }
                else if (comp == "movementComponent")
                    {
                        sf::Vector2f impulse(0, 0);

                        impulse.x = root[objectName][comp]["impulse"]["X"].asFloat();
                        impulse.y = root[objectName][comp]["impulse"]["Y"].asFloat();

                        movementComponent *mc = new movementComponent();
                        mc->setGameObject(newObj);
                        mc->move(impulse);

                        newObj->addComponent(std::type_index(typeid(movementComponent)), mc);
                    }
                else if (comp == "collisionComponent")
                    {
                        sf::Vector2f size(root[objectName][comp]["bounding_box"]["size_X"].asFloat(), root[objectName][comp]["bounding_box"]["size_Y"].asFloat());
                        sf::Vector2f offset(root[objectName][comp]["bounding_box"]["pos_X"].asFloat(), root[objectName][comp]["bounding_box"]["pos_Y"].asFloat());

                        collisionComponent *cc = new collisionComponent();
                        cc->setBounds(size, offset);
                        cc->setGameObject(newObj);

                        newObj->addComponent(std::type_index(typeid(collisionComponent)), cc);
                    }
                else if (comp == "physicsComponent")
                    {
                        float gravity = root[objectName][comp]["gravity"].asFloat();
                        float terminalVelocity = root[objectName][comp]["terminal_velocity"].asFloat();

                        physicsComponent *pc = new physicsComponent();
                        pc->setGravity(gravity);
                        pc->setTerminalVelocity(terminalVelocity);
                        pc->setGameObject(newObj);

                        newObj->addComponent(std::type_index(typeid(physicsComponent)), pc);
                    }
            }

        _gameObjects[objectName].push_back(newObj);

        return newObj;
    }

void gameObjectFactory::initializeJsonFile(const std::string &filepath)
    {
        globals::_logger->logToConsole("Initializing \"" + filepath + "\"");

        Json::Value root;

        std::ifstream read(filepath);
        if (read.is_open())
            {
                read >> root;
            }
        read.close();

        for (auto &obj : root.getMemberNames())
            {
                for (auto &comp : root[obj].getMemberNames())
                    {
                        if (comp == "textureComponent")
                            {
                                std::string texturePath = root[obj][comp]["texture"].asString();
                                _textureManager.add(texturePath, obj + "Texture");
                            }
                    }
            }
    }

std::vector<gameObject*> *gameObjectFactory::getGameObjects(const std::string &objectName)
    {
        return &_gameObjects[objectName];
    }

std::unordered_map<std::string, std::vector<gameObject*>> *gameObjectFactory::getGameObjects()
    {
        return &_gameObjects;
    }

gameObjectFactory::~gameObjectFactory()
    {
        globals::_logger->logToConsole("Cleaning Up Game Object Factory");

        for (auto &ent : _gameObjects)
            {
                for (auto &entList : ent.second)
                    {
                        if (entList)
                            {
                                delete entList;
                                entList = nullptr;
                            }
                    }
            }
    }
