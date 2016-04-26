#include "gameObject.hpp"

unsigned int gameObject::_allIDS = 0;

gameObject::gameObject(const std::string &name)
    {
        _name = name;
        _ID = _allIDS++;
    }

const std::string gameObject::getName() const
    {
        return _name;
    }

const unsigned int gameObject::getID() const
    {
        return _ID;
    }

void gameObject::addComponent(std::type_index index, component *comp)
    {
        _components[index] = comp;
    }

gameObject::~gameObject()
    {
        globals::_logger->logToConsole("Cleaning up game object \"" + _name + "\"");

        for (auto &comp : _components)
            {
                delete comp.second;
                comp.second = nullptr;
            }
    }