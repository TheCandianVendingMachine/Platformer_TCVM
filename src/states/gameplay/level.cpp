#include "level.hpp"

#include "../../utilities/collision.hpp"
#include "../../entity/entity.hpp"
#include "../../entity/platform.hpp"

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
    
    }

void level::save(const std::string &levelPath)
    {
    
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
