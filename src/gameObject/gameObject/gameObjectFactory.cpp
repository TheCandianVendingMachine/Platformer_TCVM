#include "gameObjectFactory.hpp"
#include "gameObject.hpp"

#include "../game/globals.hpp"
#include "../managers/scriptManager/scriptManager.hpp"
#include "../managers/events/eventManager.hpp"
#include "../managers/input/inputManager.hpp"

#include "../components/components.hpp"

#include "../utilities/json/json/json.h"

#include "../utilities/logger/logger.hpp"
#include <fstream>
#include <exception>

void gameObjectFactory::loadJsonFile(const std::string &file, Json::Value *root)
    {
        std::ifstream read(file);
        try
            {
                read >> *root;
            }
        catch (std::exception &e)
            {
                globals::_logger->log(file + e.what());
            }
        read.close();
    }

gameObjectFactory::gameObjectFactory()
    {
        globals::_eventManager->subscribe(this, events::LOAD_ENTITY_LIST);
        globals::_eventManager->subscribe(this, events::RELOAD_ENTITY_LIST);
    }

gameObject *gameObjectFactory::addGameObject(const std::string &objectName)
    {
        Json::Value root;

        for (auto &init : _initializedFiles)
            {
                loadJsonFile(init.first, &root);
            
                bool addedComp = false;

                gameObject *newObj = new gameObject(objectName);
                for (auto &comp : root[objectName].getMemberNames())
                    {
                        if (comp == "textureComponent")
                            {
                                addedComp = true;

                                std::string texturePath = root[objectName]["textureComponent"]["texture"].asString();
                                std::string textureName = root[objectName]["textureComponent"]["texture_name"].asString();

                                sf::Texture *texture;
                                textureComponent *tc = new textureComponent;
								tc->setGameObject(newObj);
                                texture = _textureManager.get(textureName, false);
                                if (texture)
                                    {
                                        tc->setTexture(*texture);
                                    }

                                newObj->addComponent(std::type_index(typeid(textureComponent)), tc);
                            }
                        else if (comp == "movementComponent")
                            {
                                addedComp = true;

                                sf::Vector2f impulse(0, 0);

                                impulse.x = root[objectName][comp]["impulse"]["X"].asFloat();
                                impulse.y = root[objectName][comp]["impulse"]["Y"].asFloat();

                                float maxSpeed = root[objectName][comp]["max_speed"].asFloat();
                                float acceleration = root[objectName][comp]["acceleration"].asFloat();

                                movementComponent *mc = new movementComponent();
                                mc->setGameObject(newObj);
                                mc->move(impulse);

                                mc->setAcceleration(acceleration);
                                mc->setMaxSpeed(maxSpeed);

                                newObj->addComponent(std::type_index(typeid(movementComponent)), mc);
                            }
                        else if (comp == "collisionComponent")
                            {
                                addedComp = true;

                                sf::Vector2f size(root[objectName][comp]["bounding_box"]["size_X"].asFloat(), root[objectName][comp]["bounding_box"]["size_Y"].asFloat());
                                sf::Vector2f offset(root[objectName][comp]["bounding_box"]["pos_X"].asFloat(), root[objectName][comp]["bounding_box"]["pos_Y"].asFloat());

                                std::string scriptPath = root[objectName][comp]["on_collide_script"].asString();
                                std::string scriptName = root[objectName][comp]["on_collide_func_name"].asString();


                                collisionComponent *cc = new collisionComponent();
                                if (!scriptPath.empty() || !scriptName.empty())
                                    {
                                        globals::_scriptManager->registerLuaFunction(objectName + scriptName, scriptPath, scriptName);
                                        cc->setOnCollisionScript(objectName + scriptName);
                                    }
                                cc->setBounds(size, offset);
                                cc->setGameObject(newObj);

                                newObj->addComponent(std::type_index(typeid(collisionComponent)), cc);
                            }
                        else if (comp == "physicsComponent")
                            {
                                addedComp = true;

                                float gravity = root[objectName][comp]["gravity"].asFloat();
                                float terminalVelocity = root[objectName][comp]["terminal_velocity"].asFloat();

                                float friction = root[objectName][comp]["friction_coefficient"].asFloat();

                                physicsComponent *pc = new physicsComponent();
                                pc->setGravity(gravity);
                                pc->setTerminalVelocity(terminalVelocity);
                                pc->setFriction(friction);
                                pc->setGameObject(newObj);

                                newObj->addComponent(std::type_index(typeid(physicsComponent)), pc);
                            }
                        else if (comp == "inputComponent")
                            {
                                addedComp = true;

                                inputComponent *ic = new inputComponent();
								newObj->addComponent(std::type_index(typeid(inputComponent)), ic);

                                for (auto &control : root[objectName][comp].getMemberNames())
                                    {
                                        sf::Keyboard::Key key = ic->setKey(root[objectName][comp][control]["key"].asString());
                                        
                                        bool startReal = !root[objectName][comp][control]["func_start"].isNull();
                                        bool endReal = !root[objectName][comp][control]["func_end"].isNull();

                                        if (startReal)
                                            {
                                                std::string controlName = root[objectName][comp][control]["key"].asString() + "_start";
												std::string scriptPath = root[objectName][comp][control]["script"].asString();
												std::string scriptName = root[objectName][comp][control]["func_start"].asString();
                                                ic->setFuncCallStart(controlName);
                                                auto luaCall = globals::_scriptManager->registerLuaFunction(controlName, scriptPath, scriptName);
                                                globals::_keyboardManager->add(control, key, [newObj, luaCall] () 
                                                    { 
														if (luaCall) 
															{
																(*luaCall)(*newObj->getGameObjectHandle());
															}
                                                    }, true, GAME_STATE);
                                            }

                                        if (endReal)
                                            {
												std::string controlName = root[objectName][comp][control]["key"].asString() + "_end";
												std::string scriptPath = root[objectName][comp][control]["script"].asString();
												std::string scriptName = root[objectName][comp][control]["func_end"].asString();
                                                ic->setFuncCallEnd(controlName);
                                                auto luaCall = globals::_scriptManager->registerLuaFunction(controlName, scriptPath, scriptName);
                                                globals::_keyboardManager->add(control, key, [newObj, luaCall] ()
                                                    {
                                                        if (luaCall) 
															{
																(*luaCall)(*newObj->getGameObjectHandle());
															}
                                                    }, false, GAME_STATE);
                                            }
                                    }

                                int i = 2;
                            }
                    }

                if (addedComp)
                    {
                        _gameObjects[objectName].push_back(newObj);
                        return newObj;
                    }
            }

        globals::_logger->log("Cannot add \"" + objectName + "\". It does not exist in initialized files");
        return nullptr;
    }

void gameObjectFactory::removeGameObject(gameObject *obj)
    {
        for (auto &gameObj : _gameObjects)
            {
                auto it = std::remove_if(gameObj.second.begin(), gameObj.second.end(), [&obj] (gameObject *eObj) { return eObj->getID() == obj->getID(); });
                if (it != gameObj.second.end())
                    {
                        gameObj.second.erase(it);
                        return;
                    }
            }
    }

void gameObjectFactory::initializeJsonFile(const std::string &filepath)
    {
        globals::_logger->logToConsole("Initializing \"" + filepath + "\"");
        Json::Value root;
        loadJsonFile(filepath, &root);

        for (auto &obj : root.getMemberNames())
            {
                for (auto &comp : root[obj].getMemberNames())
                    {
                        if (comp == "textureComponent")
                            {
                                std::string texturePath = root[obj][comp]["texture"].asString();
                                std::string textureName = root[obj][comp]["texture_name"].asString();
                                _textureManager.add(texturePath, textureName);
                            }
                    }

                _initializedFiles[filepath].push_back(obj);
            }
    }

void gameObjectFactory::deInitializeJsonFile(const std::string &filepath)
    {
        globals::_logger->logToConsole("Deinitializing \"" + filepath + "\"");

        Json::Value root;
        loadJsonFile(filepath, &root);

        _gameObjects.clear();

        for (auto &obj : root.getMemberNames())
            {
                for (auto &comp : root[obj].getMemberNames())
                    {
                        if (comp == "textureComponent")
                            {
                                _textureManager.remove(root[obj][comp]["texture_name"].asString());
                            }
                    }
            }

        _initializedFiles.erase(_initializedFiles.find(filepath));
    }

void gameObjectFactory::deInitializeJsonFile()
    {
        globals::_logger->logToConsole("Deinitializing all loaded Json files");
        _textureManager.removeAll();
    }

std::vector<gameObject*> *gameObjectFactory::getGameObjects(const std::string &objectName)
    {
        return &_gameObjects[objectName];
    }

std::unordered_map<std::string, std::vector<gameObject*>> *gameObjectFactory::getGameObjects()
    {
        return &_gameObjects;
    }

std::unordered_map<std::string, std::vector<std::string>> *gameObjectFactory::getInitializedEntities()
    {
        return &_initializedFiles;
    }

void gameObjectFactory::alert(eventData _data)
    {
        switch (_data._event)
            {
                case RELOAD_ENTITY_LIST:
                    for (auto &entList : _initializedFiles)
                        {
                            deInitializeJsonFile(entList.first);
                            initializeJsonFile(entList.first);
                        }
                    break;
                case LOAD_ENTITY_LIST:
                    initializeJsonFile(_data._data.stringDat);
                    break;
                default:
                    break;
            }
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

        globals::_eventManager->unsubscribe(this, events::LOAD_ENTITY_LIST);
        globals::_eventManager->unsubscribe(this, events::RELOAD_ENTITY_LIST);
    }
