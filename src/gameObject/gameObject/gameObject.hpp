// gameObject.hpp
// all things in the game. ECS being used for all entities in the game
// gameObject factory will initialize Json files of entities, and
// allow for easy use.
#pragma once

#include <map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <string>

#include "../../managers/scriptManager/luaGameObject.hpp"

class component;

class gameObject
    {
        private:
            std::map<std::type_index, component*> _components;
            std::string _name;

            unsigned int _ID;
            static unsigned int _allIDS;

            luaGameObject _luaGameObject;

        public:
            gameObject(const std::string &name);

            const std::string getName() const;
            const unsigned int getID() const;

            void addComponent(std::type_index index, component *comp);
            template<typename T>
            T *get();
			std::map<std::type_index, component*> *getAllComponents();

            luaGameObject *getGameObjectHandle();

            ~gameObject();

    };

#include "gameObject.inl"