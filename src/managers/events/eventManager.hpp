// eventManager.hpp
// manages observers, and sends them events when they happen
#pragma once

#include "events.hpp"

#include <unordered_map>
#include <vector>

class observer;

class eventManager
    {
        private:
            std::unordered_map<events, std::vector<observer*>> _observers;

        public:
            void subscribe(observer *obv, events event);
            void unsubscribe(observer *obv, events event);

            void alert(eventData data);

    };