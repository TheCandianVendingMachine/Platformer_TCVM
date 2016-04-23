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
