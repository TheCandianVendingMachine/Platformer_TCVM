#include "../components/component.hpp"
#include "../../game/globals.hpp"

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