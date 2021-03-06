#include "inputComponent.hpp"
#include "../game/globals.hpp"
#include "../managers/input/inputManager.hpp"

#include "../utilities/loadJsonFile.hpp"

sf::Keyboard::Key inputComponent::setKey(const std::string &key)
    {
        Json::Value config;
        ljf::loadJsonFile("assets/config/controls.json", &config);
        
        auto keyNum = config.get(key, config).asInt();

        return sf::Keyboard::Key(keyNum);
    }

void inputComponent::addControl(const std::string &controlName, sf::Keyboard::Key key, bool onPress, states state)
	{
		globals::_keyboardManager->add(controlName, key, onPress, state);
		_allControls.push_back(controlName);
	}

inputComponent::~inputComponent()
	{
		for (auto &key : _allControls)
			{
				globals::_keyboardManager->remove(key);
			}
	}
