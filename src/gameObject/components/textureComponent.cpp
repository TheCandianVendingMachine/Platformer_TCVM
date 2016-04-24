#include "textureComponent.hpp"
#include "collisionComponent.hpp"
#include "../gameObject/gameObject.hpp"

void textureComponent::setTexture(sf::Texture texture)
    {
        _texture = texture;
        _sprite.setTexture(_texture);
    }

sf::Texture *textureComponent::getTexture()
    {
        return &_texture;
    }

sf::Sprite *textureComponent::getSprite()
    {
        return &_sprite;
    }

void textureComponent::setPosition(float X, float Y)
    {
        _sprite.setPosition(X, Y);
        auto cc = _obj->get<collisionComponent>();
        if (cc)
            {
                cc->update();
            }
    }

void textureComponent::setPosition(sf::Vector2f pos)
    {
        _sprite.setPosition(pos);
        auto cc = _obj->get<collisionComponent>();
        if (cc)
            {
                cc->update();
            }
    }

void textureComponent::draw(sf::RenderWindow &app)
    {
        app.draw(_sprite);
    }
