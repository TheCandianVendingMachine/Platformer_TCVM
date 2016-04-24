// component.hpp
// base class for all components
#pragma once

#include <SFML/System/Time.hpp>

class gameObject;

class component
    {
        protected:
            gameObject *_obj;
            
        public:
            void setGameObject(gameObject *obj) { _obj = obj; };
            virtual ~component() {}

    };