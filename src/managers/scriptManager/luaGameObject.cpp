#include "luaGameObject.hpp"
#include "../../gameObject/gameObject/gameObject.hpp"
#include "../../gameObject/components/components.hpp"

#include "../../game/globals.hpp"
#include "../../states/stateMachine.hpp"

#include <iostream>

luaGameObject::luaGameObject(gameObject *obj)
    {
        _obj = obj;
    }

void luaGameObject::setImpulse(float X, float Y)
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                mc->move(sf::Vector2f(X, Y));
            }
    }

float luaGameObject::getImpulseX()
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                return mc->getImpulse().x;
            }
    }

float luaGameObject::getImpulseY()
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                return mc->getImpulse().y;
            }
    }

void luaGameObject::setPos(float X, float Y)
	{
		auto tc = _obj->get<textureComponent>();
		if (tc)
			{
				tc->setPosition(X, Y);
			}
	}

float luaGameObject::getPosX()
	{
		auto tc = _obj->get<textureComponent>();
		if (tc)
			{
				tc->getSprite()->getPosition().x;
			}
		return 0.0f;
	}

float luaGameObject::getPosY()
	{
		auto tc = _obj->get<textureComponent>();
		if (tc)
			{
				tc->getSprite()->getPosition().y;
			}
		return 0.0f;
	}

void luaGameObject::setSize(float X, float Y)
	{
		auto tc = _obj->get<textureComponent>();
		if (tc)
			{
				tc->setSize(X, Y);
			}
	}

float luaGameObject::getSizeX()
	{
		auto tc = _obj->get<textureComponent>();
		if (tc)
			{
				tc->getSize().x;
			}
		return 0.0f;
	}

float luaGameObject::getSizeY()
	{
		auto tc = _obj->get<textureComponent>();
		if (tc)
			{
				tc->getSize().y;
			}
		return 0.0f;
	}

float luaGameObject::getDeltaTime()
	{
		return globals::_stateMachine->getDeltaTime();
	}

void luaGameObject::test()
    {
        std::cout << "Test." << std::endl;
    }
