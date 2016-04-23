#include "globals.hpp"

resourceManager<sf::Font> *globals::_fontManager = nullptr;
resourceManager<sf::Texture> *globals::_textureManager = nullptr;

inputManager<sf::Keyboard::Key> *globals::_keyboardManager = nullptr;
inputManager<sf::Mouse::Button> *globals::_mouseManager = nullptr;

stateMachine *globals::_stateMachine = nullptr;
eventManager *globals::_eventManager = nullptr;
logr::logger *globals::_logger = nullptr;

states globals::_gameStates;