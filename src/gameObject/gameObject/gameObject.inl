#include "../components/component.hpp"
#include "gameObject.hpp"

#include "../../game/globals.hpp"

inline gameObject::gameObject(const std::string &name)
    {
        _name = name;
    }

inline const std::string gameObject::getName() const
    {
        return _name;
    }

inline void gameObject::addComponent(std::type_index index, component *comp)
    {
        _components[index] = comp;
    }

template<typename T>
T *gameObject::get()
    {
        auto it = _components.find(std::type_index(typeid(T)));
        if (it != _components.end())
            {
                return dynamic_cast<T*>(it->second);
            }

        return nullptr;
    }

inline gameObject::~gameObject()
    {
        globals::_logger->logToConsole("Cleaning up game object \"" + _name + "\"");

        for (auto &comp : _components)
            {
                delete comp.second;
                comp.second = nullptr;
            }
    }