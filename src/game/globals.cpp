#include "globals.hpp"

#include "../utilities/logger/logger.hpp"
#include "../states/gameStates/stateMachine.hpp"
#include "../managers/events/eventManager.hpp"
#include "../managers/input/inputManager.hpp"
#include "../managers/resourceManager.hpp"
#include "../game/console.hpp"

console *globals::_console = nullptr;

scriptManager *globals::_scriptManager = nullptr;

inputManager<sf::Keyboard::Key> *globals::_keyboardManager = nullptr;
inputManager<sf::Mouse::Button> *globals::_mouseManager = nullptr;

stateMachine *globals::_stateMachine = nullptr;
eventManager *globals::_eventManager = nullptr;
logr::logger *globals::_logger = nullptr;

states globals::_gameStates;