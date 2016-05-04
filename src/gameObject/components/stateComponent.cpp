#include "stateComponent.hpp"

void stateComponent::setState(const std::string &state)
	{
		if (state == "walking")
			{
				_currentState = WALKING;
			}
		else if (state == "jumping")
			{
				_currentState = JUMPING;
			}
		else if (state == "in_air")
			{
				_currentState = IN_AIR;
			}
	}

void stateComponent::setState(stateComponent::states state)
	{
		_currentState = state;
	}

stateComponent::states stateComponent::getState()
	{
		return _currentState;
	}

std::string stateComponent::getCurrentStateAsString()
	{
		std::string currentState = "";
		switch (_currentState)
			{
				case stateComponent::JUMPING:
					currentState = "jumping";
					break;
				case stateComponent::IN_AIR:
					currentState = "in_air";
					break;
				case stateComponent::WALKING:
					currentState = "walking";
					break;
				default:
					break;
			}

		return currentState;
	}
