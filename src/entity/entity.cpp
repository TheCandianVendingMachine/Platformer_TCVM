#include "entity.hpp"
#include <SFML/Graphics.hpp>

unsigned int entity::_allIDs;

entity::entity() : _ID(_allIDs++)
	{
	}

void entity::draw(sf::RenderTarget &target)
    {
        target.draw(_sprite);
    }

void entity::setPosition(sf::Vector2f pos)
    {
        _sprite.setPosition(pos);
    }

void entity::setPosition(float X, float Y)
    {
        _sprite.setPosition(X, Y);
    }

sf::Vector2f entity::getSize()
    {
        return sf::Vector2f(_sprite.getGlobalBounds().width, _sprite.getGlobalBounds().height);
    }

void entity::setSize(float X, float Y)
    {
        _sprite.scale(X / _sprite.getScale().x, Y / _sprite.getScale().y);
    }

void entity::setSize(sf::Vector2f size)
    {
        _sprite.scale(size.x / _sprite.getScale().x, size.y / _sprite.getScale().y);
    }

const entity::entityID entity::getEntityID() const
	{
		return _entID;
	}

const unsigned int entity::getID() const
    {
        return _ID;
    }

sf::Vector2f entity::getPosition()
    {
        return _sprite.getPosition();
    }

sf::Sprite *entity::getSprite()
    {
        return &_sprite;
    }

entity::~entity()
    {}
