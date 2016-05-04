// stateComponent.hpp
// allows the entity to be in multiple states.
#pragma once

#include "component.hpp"
#include <string>

class stateComponent : public component
	{
		public:
			enum states
				{
					JUMPING,
					IN_AIR,
					WALKING,
				};
		private:
			states _currentState;

		public:
			void setState(const std::string &state);
			void setState(states state);

			states getState();
			std::string getCurrentStateAsString();

	};