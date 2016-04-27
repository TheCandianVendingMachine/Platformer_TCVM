#include "luaGameObject.hpp"
#include "../../gameObject/gameObject/gameObject.hpp"
#include "../../gameObject/components/components.hpp"

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

void luaGameObject::test()
    {
        std::cout << "Test." << std::endl;
    }
