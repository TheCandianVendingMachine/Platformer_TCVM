#include "player.hpp"

#include "../game/globals.hpp"

player::player() : _acceleration(50.f), _gravity(500.f), _maxVelocity(400.f), _terminalVelocity(700.f)
    {
        _entID = PLAYER;
        _impulse = sf::Vector2f(0, 0);

        _sprite.setTexture(*globals::_textureManager->get("playerTexture", false));

        _enablePhysics = true;

        globals::_keyboardManager->changeFunction("player_jump", [this] () 
            {
                if (!_jumping)
                    {
                        _enablePhysics = true;
                        _jumping = true;
                        _impulse.y = -500.f;
                    }
            });

        globals::_keyboardManager->changeFunction("player_move_left", [this] () { _impulse.x += -_acceleration; });
        globals::_keyboardManager->changeFunction("player_move_right", [this] () { _impulse.x += _acceleration; });
    }

void player::update(sf::Time deltaTime)
    {
        if (_enablePhysics)
            {
                _impulse.y += _gravity * deltaTime.asSeconds();
                if (_impulse.y >= _terminalVelocity)
                    {
                        _impulse.y = 700;
                    }
        }

        if (abs(_impulse.x) > 0)
            {
                _impulse.x > 0 ? _impulse.x -= _acceleration / 4.f : _impulse.x += _acceleration / 4.f;
            }

        if (abs(_impulse.x) >= _maxVelocity)
            {
                _impulse.x > 0 ? _impulse.x = _maxVelocity : _impulse.x = -_maxVelocity;
            }

        _sprite.move(_impulse * deltaTime.asSeconds());
    }

void player::onCollide()
    {
        _enablePhysics = false;
        _jumping = false;
        _impulse.y = 0;
    }

void player::offCollide()
    {
        _enablePhysics = true;
    }
