#include "functionFrontends.hpp"

#include "../game/globals.hpp"
#include "../managers/events/eventManager.hpp"

namespace frontendFuncs
    {
        void alert(const std::string &event)
            {
                events eventUsed;
                if (event == "next_level")
                    {
                        eventUsed = NEXT_LEVEL;
                    }

                globals::_eventManager->alert(eventData(eventUsed));
            }
    }