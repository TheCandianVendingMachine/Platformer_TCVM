#include "eventManager.hpp"
#include "observer.hpp"

void eventManager::subscribe(observer *obv, events event)
    {
		auto it = std::find(_observers[event].begin(), _observers[event].end(), obv);
		if (it == _observers[event].end()) 
			{
				_observers[event].push_back(obv);
			}
    }

void eventManager::unsubscribe(observer *obv, events event)
    {
        auto it = _observers.find(event);
        if (it != _observers.end())
            {
                auto itVec = std::find(it->second.begin(), it->second.end(), obv);
                if (itVec != it->second.end())
                    {
                        it->second.erase(itVec);
                    }
            }
    }

void eventManager::alert(eventData _event)
    {
        auto it = _observers.find(_event._event);
        if (it != _observers.end())
            {
                for (auto &obv : it->second)
                    {
                        obv->alert(_event);
                    }
            }
    }
