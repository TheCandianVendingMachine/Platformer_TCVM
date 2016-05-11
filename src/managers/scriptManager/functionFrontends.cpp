#include "functionFrontends.hpp"

#include "../game/globals.hpp"
#include "../utilities/logger/logger.hpp"
#include "../states/gameStates/stateMachine.hpp"
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

        float getDeltaTime()
            {
                return globals::_stateMachine->getDeltaTime();
            }

        void log(const std::string &message)
            {
                globals::_logger->log(message);
            }

        void logToConsole(const std::string &message)
            {
                globals::_logger->logToConsole(message);
            }

    }