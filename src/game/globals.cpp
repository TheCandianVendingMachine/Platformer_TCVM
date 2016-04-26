#include "globals.hpp"

inputManager<sf::Keyboard::Key> *globals::_keyboardManager = nullptr;
inputManager<sf::Mouse::Button> *globals::_mouseManager = nullptr;

stringInputManager *globals::_textEntered = nullptr;

stateMachine *globals::_stateMachine = nullptr;
eventManager *globals::_eventManager = nullptr;
logr::logger *globals::_logger = nullptr;

states globals::_gameStates;