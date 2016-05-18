#include "gameObjectFactory.hpp"
#include "gameObject.hpp"

#include "../game/globals.hpp"
#include "../managers/scriptManager/scriptManager.hpp"
#include "../managers/events/eventManager.hpp"
#include "../managers/input/inputManager.hpp"
#include "../states/gameStates/stateMachine.hpp"

#include "../components/components.hpp"

#include "../utilities/loadJsonFile.hpp"

#include <algorithm>

gameObject *gameObjectFactory::fetchFromPool(const std::string &name)
    {
        gameObject *retObj = nullptr;

        auto it = std::find_if(_gameObjectPool.begin(), _gameObjectPool.end(), [name] (gameObject *obj) { return obj->getName() == name; });
        if (it != _gameObjectPool.end())
            {
                retObj = *it;
            }
        
        return retObj;
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
                ljf::loadJsonFile(init.first, &root);
            
                bool addedComp = false;
                bool addComponent = true;

                auto obj = fetchFromPool(objectName);
                if (obj)
                    {
                        _gameObjectPool.erase(std::find_if(_gameObjectPool.begin(), _gameObjectPool.end(), [obj] (gameObject *objPool) { return objPool->getID() == obj->getID(); }));
                        addComponent = false;
                    }
                else
                    {
                        obj = new gameObject(objectName);
                    }
                        
                
                for (auto &comp : root[objectName].getMemberNames())
                    {
                        if (comp == "textureComponent")
                            {
                                addedComp = true;

                                std::string texturePath = root[objectName]["textureComponent"]["texture"].asString();
                                std::string textureName = root[objectName]["textureComponent"]["texture_name"].asString();

                                sf::Texture *texture = _textureManager.get(textureName, false);

                                textureComponent *tc = nullptr;
                                if (addComponent)
                                    {
                                        tc = new textureComponent;
                                        obj->addComponent(std::type_index(typeid(textureComponent)), tc);
                                    }
                                else
                                    {
                                        tc = obj->get<textureComponent>();
                                    }

                                tc->setGameObject(obj);
                                
                                if (texture)
                                    {
                                        tc->setTexture(*texture);
                                        tc->setSize(static_cast<sf::Vector2f>(tc->getTexture()->getSize()));
                                        tc->setRotation(0);
                                    }

                                obj->addComponent(std::type_index(typeid(textureComponent)), tc);
                            }
                        else if (comp == "movementComponent")
                            {
                                addedComp = true;

                                sf::Vector2f impulse(0, 0);

                                impulse.x = root[objectName][comp]["impulse"]["X"].asFloat();
                                impulse.y = root[objectName][comp]["impulse"]["Y"].asFloat();

                                float maxSpeed = root[objectName][comp]["max_speed"].asFloat();
                                float acceleration = root[objectName][comp]["acceleration"].asFloat();

                                movementComponent *mc = nullptr;
                                if (addComponent)
                                    {
                                        mc = new movementComponent();
                                        obj->addComponent(std::type_index(typeid(movementComponent)), mc);
                                    }
                                else
                                    {
                                        mc = obj->get<movementComponent>();
                                    }

                                mc->setGameObject(obj);
                                mc->move(impulse);

                                mc->setAcceleration(acceleration);
                                mc->setMaxSpeed(maxSpeed);
                            }
                        else if (comp == "collisionComponent")
                            {
                                addedComp = true;

                                sf::Vector2f size(root[objectName][comp]["bounding_box"]["size_X"].asFloat(), root[objectName][comp]["bounding_box"]["size_Y"].asFloat());
                                sf::Vector2f offset(root[objectName][comp]["bounding_box"]["pos_X"].asFloat(), root[objectName][comp]["bounding_box"]["pos_Y"].asFloat());

                                std::string scriptPath = root[objectName][comp]["on_collide_script"].asString();
                                std::string scriptName = root[objectName][comp]["on_collide_func_name"].asString();

                                std::string surfaceType = root[objectName][comp]["surface_type"].asString();

                                collisionComponent *cc = nullptr;
                                if (addComponent)
                                    {
                                        cc = new collisionComponent;
                                        obj->addComponent(std::type_index(typeid(collisionComponent)), cc);
                                    }
                                else
                                    {
                                        cc = obj->get<collisionComponent>();
                                    }

                                if (surfaceType == "collidable")
                                    {
                                        cc->setSurfaceType(collisionComponent::COLLIDABLE);
                                    }
                                else if (surfaceType == "non_collidable")
                                    {
                                        cc->setSurfaceType(collisionComponent::NON_COLLIDABLE);
                                    }

                                if (!scriptPath.empty() || !scriptName.empty())
                                    {
                                        globals::_scriptManager->registerLuaFunction(objectName + "_" + scriptName, scriptPath, scriptName);
                                        cc->setOnCollisionScript(objectName + "_" + scriptName);
                                    }
                                cc->setBounds(size, offset);
                                cc->setGameObject(obj);
                            }
                        else if (comp == "physicsComponent")
                            {
                                addedComp = true;

                                float gravity = root[objectName][comp]["gravity"].asFloat();
                                float terminalVelocity = root[objectName][comp]["terminal_velocity"].asFloat();

                                float friction = root[objectName][comp]["friction_coefficient"].asFloat();

                                physicsComponent *pc = nullptr;
                                if (addComponent)
                                    {
                                        pc = new physicsComponent;
                                        obj->addComponent(std::type_index(typeid(physicsComponent)), pc);
                                    }
                                else
                                    {
                                        pc = obj->get<physicsComponent>();
                                    }
                                pc->setGravity(gravity);
                                pc->setTerminalVelocity(terminalVelocity);
                                pc->setFriction(friction);
                                pc->setGameObject(obj);
                            }
                        else if (comp == "inputComponent")
                            {
                                addedComp = true;

                                inputComponent *ic = nullptr;
                                if (addComponent)
                                    {
                                        ic = new inputComponent;
                                        obj->addComponent(std::type_index(typeid(inputComponent)), ic);
                                    }
                                else
                                    {
                                        ic = obj->get<inputComponent>();
                                    }
                                
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
												ic->addControl(controlName, key, true, GAME_STATE);
												auto luaCall = globals::_scriptManager->registerLuaFunction(controlName, scriptPath, scriptName);
												globals::_keyboardManager->changeFunction(controlName, [obj, luaCall] ()
                                                    { 
														if (luaCall) 
															{
																(*luaCall)(*obj->getGameObjectHandle());
															}
                                                    });
                                            }

                                        if (endReal)
                                            {
												std::string controlName = root[objectName][comp][control]["key"].asString() + "_end";
												std::string scriptPath = root[objectName][comp][control]["script"].asString();
												std::string scriptName = root[objectName][comp][control]["func_end"].asString();
												ic->addControl(controlName, key, false, GAME_STATE);
												auto luaCall = globals::_scriptManager->registerLuaFunction(controlName, scriptPath, scriptName);
												globals::_keyboardManager->changeFunction(controlName, [obj, luaCall] ()
                                                    { 
														if (luaCall) 
															{
																(*luaCall)(*obj->getGameObjectHandle());
															}
                                                    });
                                            }
                                    }
                            }
						else if (comp == "cameraComponent")
							{
                                addedComp = true;

                                cameraComponent *cc = nullptr;
                                if (addComponent)
                                    {
                                        cc = new cameraComponent;
                                        obj->addComponent(std::type_index(typeid(cameraComponent)), cc);
                                    }
                                else
                                    {
                                        cc = obj->get<cameraComponent>();
                                    }

								sf::Vector2f camSize(root[objectName][comp]["size"]["X"].asFloat(),
													    root[objectName][comp]["size"]["Y"].asFloat());
								sf::Vector2f camOffset(root[objectName][comp]["offset"]["X"].asFloat(),
													    root[objectName][comp]["offset"]["Y"].asFloat());

                                float followRadius = root[objectName][comp]["follow_radius"].asFloat();

								bool follow = root[objectName][comp]["follow_object"].asBool();

								cc->setGameObject(obj);
								cc->setWindow(globals::_stateMachine->getWindow());
								cc->setCameraSize(camSize);
								cc->setCameraOffset(camOffset);
                                cc->setFollowRadius(followRadius);
								cc->setFollow(follow);
							}
                        else if (comp == "stateComponent")
                            {
                                addedComp = true;

                                stateComponent *sc = nullptr;
                                if (addComponent)
                                    {
                                        sc = new stateComponent;
                                        obj->addComponent(std::type_index(typeid(stateComponent)), sc);
                                    }
                                else
                                    {
                                        sc = obj->get<stateComponent>();
                                    }

                                sc->setGameObject(obj);
                            }
                    }

                if (addedComp)
                    {
                        _gameObjects[objectName].push_back(obj);
                        return obj;
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
                        _gameObjectPool.push_back(*it);
                        gameObj.second.erase(it);
                        return;
                    }
            }
    }

void gameObjectFactory::initializeJsonFile(const std::string &filepath)
    {
        globals::_logger->logToConsole("Initializing \"" + filepath + "\"");
        Json::Value root;
        ljf::loadJsonFile(filepath, &root);

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
        ljf::loadJsonFile(filepath, &root);

        for (auto &obj : root.getMemberNames())
            {
                for (auto &comp : root[obj].getMemberNames())
                    {
                        if (comp == "textureComponent")
                            {
                                _textureManager.remove(root[obj][comp]["texture_name"].asString());
                                break;
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

void gameObjectFactory::clear()
    {
        for (auto &ent : _gameObjects)
            {
                for (auto &entList : ent.second)
                    {
                        if (entList)
                            {
                                _gameObjectPool.push_back(entList);
                            }
                    }

                ent.second.clear();
            }
        _gameObjects.clear();
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
        _gameObjects.clear();

        for (auto &ent : _gameObjectPool)
            {
                if (ent)
                    {
                        delete ent;
                        ent = nullptr;
                    }
            }
        _gameObjectPool.clear();

        globals::_eventManager->unsubscribe(this, events::LOAD_ENTITY_LIST);
        globals::_eventManager->unsubscribe(this, events::RELOAD_ENTITY_LIST);
    }
