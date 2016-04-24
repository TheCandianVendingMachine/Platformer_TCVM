#include "level.hpp"

#include "../../utilities/collision.hpp"
#include "../../entity/entity.hpp"
#include "../../entity/platform.hpp"

#include <json/json.h>

void level::draw(sf::RenderWindow &app)
    {
        for (auto &platform : _platforms)
            {
                platform->draw(app);
            }
    }

void level::handleCollision(entity *collider)
    {
        for (auto &platform : _platforms)
            {
                auto platformPos = platform->getPosition();
                auto platformBounds = platform->getSprite()->getGlobalBounds();
                if (clsn::handleCollision(collider, &*platform))
                    {
                        collider->onCollide();
                    }
                else
                    {
                        collider->offCollide();
                    }
            }
    }

void level::load(const std::string &levelPath)
    {
        _platforms.clear();

        Json::Value root;
        std::ifstream read(levelPath);
        if (read.is_open())
            {
                read >> root;
            }
        read.close();

        auto members = root["level"]["platforms"].getMemberNames();
        for (auto &platform : members)
            {
                auto platformArr = root["level"]["platforms"][platform];

                sf::Vector2f pos(platformArr["position"]["X"].asFloat(), platformArr["position"]["Y"].asFloat());
                sf::Vector2f size(platformArr["size"]["X"].asFloat(), platformArr["size"]["Y"].asFloat());
                float angle = platformArr["rotation"].asFloat();

                auto _platform = addPlatform();
                _platform->setPosition(pos);
                _platform->setSize(size);
                _platform->getSprite()->setRotation(angle);
                
            }

        globals::_logger->logToConsole("Level Loaded");
    }

void level::save(const std::string &levelPath)
    {
        Json::Value root;

        root["level"]["platforms"];
        for (auto &platform : _platforms)
            {
                root["level"]["platforms"]["platform_" + std::to_string(platform->getID())]["position"]["X"] = platform->getPosition().x;
                root["level"]["platforms"]["platform_" + std::to_string(platform->getID())]["position"]["Y"] = platform->getPosition().y;

                root["level"]["platforms"]["platform_" + std::to_string(platform->getID())]["size"]["X"] = platform->getSize().x;
                root["level"]["platforms"]["platform_" + std::to_string(platform->getID())]["size"]["Y"] = platform->getSize().y;

                root["level"]["platforms"]["platform_" + std::to_string(platform->getID())]["rotation"] = platform->getSprite()->getRotation();
            }
        
        root["level"]["entity"];

        std::ofstream write(levelPath);
        if (write.is_open())
            {
                write << root;
            }
        write.close();

        globals::_logger->logToConsole("Level Saved");
    }

entity *level::addPlatform()
    {
        _platforms.push_back(std::shared_ptr<platform>(new platform));
        return &*_platforms.back();
    }

void level::removePlatform(entity *platform)
    {
        _platforms.erase(std::remove_if(_platforms.begin(), _platforms.end(), [this, &platform] (std::shared_ptr<entity> ent) { return platform->getID() == ent->getID(); }));
    }

entity *level::getPlatformAt(sf::Vector2f pos)
    {
        for (auto &platform : _platforms)
            {
                auto platformBounds = platform->getSprite()->getGlobalBounds();
                if (clsn::hasCollided(pos, sf::Vector2f(1, 1), sf::Vector2f(platformBounds.left, platformBounds.top), sf::Vector2f(platformBounds.width, platformBounds.height)))
                    {
                        return &*platform;
                    }
            }

        return nullptr;
    }
