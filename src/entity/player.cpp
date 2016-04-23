#include "player.hpp"

#include "../game/globals.hpp"

player::player()
    {
        _entID = PLAYER;
        _impulse = sf::Vector2f(0, 0);

        _sprite.setTexture(*globals::_textureManager->get("playerTexture", false));
    }

void player::update(sf::Time deltaTime)
    {
        _impulse.y += 500.f * deltaTime.asSeconds();
        if (_impulse.y >= 400)
            {
                _impulse.y = 400;
            }

        _sprite.move(_impulse * deltaTime.asSeconds());
    }

void player::collide()
    {
        _impulse.y = 0;
    }
