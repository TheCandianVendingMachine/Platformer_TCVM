// inputComponent.hpp
// allows the character to move
#pragma once

#include "component.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <vector>

#include "../states/gameStates/states.hpp"

class inputComponent : public component
    {
		private:
			std::vector<std::string> _allControls;

        public:
			sf::Keyboard::Key setKey(const std::string &key);
			void addControl(const std::string &controlName, sf::Keyboard::Key key, bool onPress, states state);

			~inputComponent();

    };