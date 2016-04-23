// observer.hpp
// the observer class. Inherit from this to be able to subscribe to the eventManager
#pragma once

#include "events.hpp"

class observer
    {
        public:
            virtual void alert(eventData _data) = 0;
			virtual ~observer() {}

    };