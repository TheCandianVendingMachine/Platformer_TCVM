#include "gameworld.hpp"
#include "../../entity/entity.hpp"

#include "../../game/globals.hpp"

gameworld::gameworld()
    {
        _entities.push_back(&_plr);
    }

void gameworld::draw()
    {
        _level.draw(*globals::_stateMachine->getWindow());
        for (auto &ent : _entities)
            {
                ent->draw(*globals::_stateMachine->getWindow());
            }

    }

void gameworld::update(sf::Time deltaTime)
    {
        for (auto &ent : _entities)
            {
                ent->update(deltaTime);
                _level.handleCollision(ent);
            }
    }

level *gameworld::getLevel()
    {
        return &_level;
    }
