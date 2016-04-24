#include "entity.hpp"
#include <SFML/Graphics.hpp>

unsigned int entity::_allIDs;

entity::entity() : _ID(_allIDs++), _entName("entity")
	{
	}

void entity::draw(sf::RenderTarget &target)
    {
        target.draw(_sprite);
    }

void entity::setTexture(sf::Texture *texture)
    {
        _sprite.setTexture(*texture);
        _size = sf::Vector2f(_sprite.getLocalBounds().width, _sprite.getLocalBounds().height);
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
        return _size;
    }

void entity::setSize(float X, float Y)
    {
        _sprite.scale(X / _sprite.getLocalBounds().width, Y / _sprite.getLocalBounds().height);
        _size = sf::Vector2f(X / _sprite.getLocalBounds().width, Y / _sprite.getLocalBounds().height);
    }

void entity::setSize(sf::Vector2f size)
    {
        _sprite.scale(size.x / _sprite.getLocalBounds().width, size.y / _sprite.getLocalBounds().height);
        _size = sf::Vector2f(size.x / _sprite.getLocalBounds().width, size.y / _sprite.getLocalBounds().height);
    }

const std::string &entity::getEntityName()
    {
        return _entName;
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
