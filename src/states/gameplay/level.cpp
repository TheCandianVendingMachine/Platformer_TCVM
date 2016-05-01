#include "level.hpp"

#include "../../gameObject/gameObject/gameObject.hpp"
#include "../../utilities/collision.hpp"
#include "../../gameObject/components/components.hpp"

#include "../../utilities/strFuncs.hpp"
#include "../../game/globals.hpp"

#include <logger.hpp>
#include <fstream>

void level::loadJsonFile(const std::string &file, Json::Value *root)
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

level::level()
    {
        _factory.initializeJsonFile("assets/entities/default_entity_list.json");
    }

void level::load(const std::string &levelPath)
    {
        _platforms.clear();
        _entities.clear();

        Json::Value root;
        std::ifstream read(levelPath);
        if (read.is_open())
            {
                read >> root;
            }
        read.close();

        auto members = root["level_entity_lists"].getMemberNames();
        for (auto &entLists : members)
            {
                _factory.initializeJsonFile(entLists);
            }

        members = root["level"]["platforms"].getMemberNames();
        for (auto &platform : members)
            {
                auto vars = root["level"]["platforms"][platform];

                auto obj = _factory.addGameObject("platform");
                auto textureComp = obj->get<textureComponent>();

                if (textureComp)
                    {
                        textureComp->setPosition(vars["position"]["X"].asFloat(), vars["position"]["Y"].asFloat());
                        textureComp->setSize(vars["size"]["X"].asFloat(), vars["size"]["Y"].asFloat());
                        textureComp->getSprite()->setRotation(vars["rotation"].asFloat());
                    }

                _platforms.push_back(obj);
            }

        members = root["level"]["entity"].getMemberNames();
        for (auto &ent : members)
            {
                if (ent == "player")
                    {
                        auto vars = root["level"]["entity"][ent];

                        auto obj = _factory.addGameObject("player");
                        auto textureComp = obj->get<textureComponent>();

                        if(textureComp)
                            {
                                textureComp->setPosition(vars["position"]["X"].asFloat(), vars["positon"]["X"].asFloat());
                            }

                        _entities.push_back(obj);
                    }
            }

        globals::_logger->logToConsole("Level Loaded");
    }

void level::save(const std::string &levelPath)
    {
        Json::Value root;

        root["level"]["platforms"];
        for (auto &platform : _platforms)
            {
                sf::Vector2f pos(0, 0);
                sf::Vector2f size(0, 0);
                float angle = 0.f;

                auto textureComp = platform->get<textureComponent>();
                if (textureComp)
                    {
                        pos = textureComp->getSprite()->getPosition();
                        size = textureComp->getSize();
                        angle = textureComp->getSprite()->getRotation();
                    }
                
                root["level"]["platforms"]["platform" + std::to_string(platform->getID())]["position"]["X"] = pos.x;
                root["level"]["platforms"]["platform" + std::to_string(platform->getID())]["position"]["Y"] = pos.y;
                root["level"]["platforms"]["platform" + std::to_string(platform->getID())]["size"]["X"] = size.x;
                root["level"]["platforms"]["platform" + std::to_string(platform->getID())]["size"]["Y"] = size.y;
                root["level"]["platforms"]["platform" + std::to_string(platform->getID())]["rotation"] = angle;
            }
        
        root["level"]["entity"];
        for (auto &ent : _entities)
            {
                sf::Vector2f pos(0, 0);

                auto textureComp = ent->get<textureComponent>();
                if (textureComp)
                    {
                        pos = textureComp->getSprite()->getPosition();
                    }

                root["level"]["entity"][ent->getName()]["position"]["X"] = pos.x;
                root["level"]["entity"][ent->getName()]["position"]["Y"] = pos.y;
            }

        Json::Value loadedRoot;
        loadJsonFile(levelPath, &loadedRoot);
        root["level_entity_lists"];
        auto members = loadedRoot["level_entity_lists"].getMemberNames();
        for (auto &entLists : members)
            {
                root["level_entity_lists"][entLists];
            }

        std::ofstream write(levelPath);
        if (write.is_open())
            {
                write << root;
            }
        write.close();

        globals::_logger->logToConsole("Level Saved");
    }

void level::update(sf::Time deltaTime)
    {
        for (auto &platform : _platforms)
            {
                auto platformMC = platform->get<movementComponent>();
                if (platformMC)
                    {
                        platformMC->update(deltaTime);
                    }
            }

        for (auto &ent : _entities)
            {
                auto mc = ent->get<movementComponent>();
                if (mc)
                    {
                        mc->update(deltaTime);
                    }

                auto cc = ent->get<collisionComponent>();
                if (cc)
                    {
                        for (auto &platform : _platforms)
                            {
                                auto platformCC = platform->get<collisionComponent>();
                                if (platformCC)
                                    {
                                        cc->collide(*platformCC);
                                    }
                            }
                    }
            }
    }

void level::draw(sf::RenderWindow &app)
    {
        for (auto &ent : _platforms)
            {
                auto textureComp = ent->get<textureComponent>();
                if (textureComp)
                    {
                        textureComp->draw(app);
                    }
            }

        for (auto &ent : _entities)
            {
                auto textureComp = ent->get<textureComponent>();
                if (textureComp)
                    {
                        textureComp->draw(app);
                    }
            }
    }

gameObject* level::addEntity(const std::string &name)
    {
        auto ent = _factory.addGameObject(name);
        if (ent)
            {
                if (name == "platform" || name == "moving_platform")
                    {
                        _platforms.push_back(ent);
                    }
                else
                    {
                        _entities.push_back(ent);
                    }
            }

        return ent;
    }

gameObject *level::getEntityAtPosition(sf::Vector2f pos)
    {
        auto allEnts = _factory.getGameObjects();
        for (auto &ent : *allEnts)
            {
                for (auto &obj : ent.second)
                    {
                        auto tc = obj->get<textureComponent>();
                        if (tc)
                            {
                                sf::Vector2f entPos = tc->getSprite()->getPosition();
                                auto cc = obj->get<collisionComponent>();
                                if (cc)
                                    {
                                        if (clsn::hasCollided(pos, sf::Vector2f(1, 1), entPos, sf::Vector2f(cc->getBounds().width, cc->getBounds().height)))
                                            {
                                                return obj;
                                            }
                                    }
                            }
                    }
            }

        return nullptr;
    }

std::unordered_map<std::string, std::vector<gameObject*>> *level::getAllGameObjects()
    {
        return _factory.getGameObjects();
    }

std::unordered_map<std::string, std::vector<std::string>> *level::getInitializedObjects()
    {
        return _factory.getInitializedEntities();
    }

void level::removeEntity(gameObject *obj)
    {
        auto itEnt = std::remove_if(_entities.begin(), _entities.end(), [&obj] (gameObject *eObj) { return eObj->getID() == obj->getID(); });
        if (itEnt != _entities.end())
            {
                _entities.erase(itEnt);
                _factory.removeGameObject(obj);
                return;
            }

        auto itPlat = std::remove_if(_platforms.begin(), _platforms.end(), [&obj] (gameObject *eObj) { return eObj->getID() == obj->getID(); });
        if (itPlat != _platforms.end())
            {
                _platforms.erase(itPlat);
                _factory.removeGameObject(obj);
                return;
            }
    }
