#include "level.hpp"

#include "../../gameObject/gameObject/gameObject.hpp"

#include "../../gameObject/components/components.hpp"

#include "../../utilities/strFuncs.hpp"
#include "../../game/globals.hpp"

#include <fstream>
#include <json/json.h>

level::level()
    {
        _currentEntityList = "assets/entities/entity_list.json";
        _factory.initializeJsonFile(_currentEntityList);
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

        _currentEntityList = root["level"]["entity_list"].asString();

        auto members = root["level"]["platforms"].getMemberNames();
        for (auto &platform : members)
            {
                auto vars = root[platform];

                auto obj = _factory.addGameObject("platform", _currentEntityList);
                auto textureComp = obj->get<textureComponent>();

                if (textureComp)
                    {
                        textureComp->setPosition(vars["position"]["X"].asFloat(), vars["position"]["Y"].asFloat());
                        textureComp->getSprite()->setRotation(vars["rotation"].asFloat());
                    }
            }

        members = root["level"]["entity"].getMemberNames();
        for (auto &ent : members)
            {
                if (ent == "player")
                    {
                        auto vars = root[ent];

                        auto obj = _factory.addGameObject("player", _currentEntityList);
                        auto textureComp = obj->get<textureComponent>();

                        if(textureComp)
                            {
                                textureComp->setPosition(vars["position"]["X"].asFloat(), vars["positon"]["X"].asFloat());
                            }
                    }
            }

        globals::_logger->logToConsole("Level Loaded");
    }

void level::save(const std::string &levelPath)
    {
        Json::Value root;

        root["level"]["entity_list"] = "assets/entities/entity_list.json";
        root["level"]["platforms"];
        for (auto &platform : _platforms)
            {
                sf::Vector2f pos(0, 0);
                float angle = 0.f;

                auto textureComp = platform->get<textureComponent>();
                if (textureComp)
                    {
                        pos = textureComp->getSprite()->getPosition();
                        angle = textureComp->getSprite()->getRotation();
                    }

                root["level"]["platforms"]["platform"]["position"]["X"] = pos.x;
                root["level"]["platforms"]["platform"]["position"]["Y"] = pos.y;
                root["level"]["platforms"]["platform"]["rotation"] = angle;
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
        auto ent = _factory.addGameObject(name, _currentEntityList);
        if (ent)
            {
                if (name == "platform")
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
