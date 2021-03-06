#include "luaGameObject.hpp"
#include "../gameObject/gameObject/gameObject.hpp"
#include "../gameObject/components/components.hpp"

#include "../utilities/logger/logger.hpp"
#include "../game/globals.hpp"
#include "../states/gameStates/stateMachine.hpp"
#include "../utilities/strFuncs.hpp"

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

		return 0.0f;
    }

float luaGameObject::getImpulseY()
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                return mc->getImpulse().y;
            }

		return 0.0f;
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
				return tc->getSprite()->getPosition().x;
			}

		return 0.0f;
	}

float luaGameObject::getPosY()
	{
		auto tc = _obj->get<textureComponent>();
		if (tc)
			{
				return tc->getSprite()->getPosition().y;
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

void luaGameObject::setAcceleration(float accel)
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                return mc->setAcceleration(accel);
            }
    }

float luaGameObject::getAcceleration()
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                return mc->getAcceleration();
            }
        return 0.0f;
    }

void luaGameObject::setMaxSpeed(float speed)
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                return mc->setMaxSpeed(speed);
            }
    }

float luaGameObject::getMaxSpeed()
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                return mc->getMaxSpeed();
            }
        return 0.0f;
    }

void luaGameObject::setCurrentState(const std::string &state)
	{
		auto sc = _obj->get<stateComponent>();
		if (sc)
			{
				sc->setState(state);
			}
	}

std::string luaGameObject::getCurrentState()
	{
		auto sc = _obj->get<stateComponent>();
		if (sc)
			{
				return sc->getCurrentStateAsString();
			}

		return "";
	}

bool luaGameObject::isType(const std::string &entityName)
	{
		return strfn::doesStringContain(_obj->getName(), entityName).second;
	}
