#include "../../game/globals.hpp"
#include "input.hpp"
#include "inputManager.hpp"

template<typename T>
void inputManager<T>::add(const std::string &name, T key, inputState inputState, states activeState)
    {
        _inputs[name] = input<T>(key, [] () {}, inputState, false, activeState);
        _inputs[name].setInverseFunction([] () {});

        _eventTimeInputs[name] = &_inputs[name];
    }

template<typename T>
void inputManager<T>::add(const std::string &name, T key, std::function<void()> onInput, inputState inputState, states activeState)
    {
        _inputs[name] = input<T>(key, onInput, inputState, false, activeState);
        _inputs[name].setInverseFunction([] () {});

        _eventTimeInputs[name] = &_inputs[name];
    }

template<typename T>
void inputManager<T>::add(const std::string &name, T key, inputState inputState, bool doubleClick, states activeState)
	{
		_inputs[name] = input<T>(key, [] () {}, inputState, doubleClick, activeState);
        _inputs[name].setInverseFunction([] () {});

        _eventTimeInputs[name] = &_inputs[name];
	}

template<typename T>
void inputManager<T>::add(const std::string &name, T key, std::function<void()> onInput, inputState inputState, bool doubleClick, states activeState)
	{
		auto it = _inputs.find(name);
        if (it == _inputs.end())
            {
                _inputs[name] = input<T>(key, onInput, doubleClick, activeState);
                _inputs[name].setInverseFunction([] () {});

				_eventTimeInputs[name] = &_inputs[name];
            }
        else
            {
                // if the input is in the map, and onPress isnt the same as the previous value, assume that the
                // user wants to change the inverse function
                if (_inputs[name].onPress() != onPress)
                    {
                        _inputs[name].setInverseFunction(onInput);
                    }
            }
	}

template<typename T>
void inputManager<T>::add(const std::string &name, T key, bool onPress, states activeState)
    {
        auto it = _inputs.find(name);
        if (it == _inputs.end())
            {
                _inputs[name] = input<T>(key, [] () {}, onPress, false, activeState);
                _inputs[name].setInverseFunction([] () {});
            }
        else
            {
                // if the input is in the map, and onPress isnt the same as the previous value, assume that the
                // user wants to change the inverse function
                if (_inputs[name].onPress() != onPress)
                    {
                        _inputs[name].setInverseFunction(onInput);
                    }
            }

        _realTimeInputs[name] = &_inputs[name];
    }

template<typename T>
void inputManager<T>::add(const std::string &name, T key, std::function<void()> onInput, bool onPress, states activeState)
    {
        auto it = _inputs.find(name);
        if (it == _inputs.end())
            {
                _inputs[name] = input<T>(key, onInput, onPress, false, activeState);
                _inputs[name].setInverseFunction([] () {});

                _realTimeInputs[name] = &_inputs[name];
            }
        else
            {
                // if the input is in the map, and onPress isnt the same as the previous value, assume that the
                // user wants to change the inverse function
                if (_inputs[name].onPress() != onPress)
                    {
                        _inputs[name].setInverseFunction(onInput);
                    }
            }
    }

template<typename T>
void inputManager<T>::add(const std::string &name, T key, bool onPress, bool doubleClick, states activeState)
	{
	    auto it = _inputs.find(name);
        if (it == _inputs.end())
            {
				_inputs[name] = input<T>(key, [] () {}, onPress, doubleClick, activeState);
                _inputs[name].setInverseFunction([] () {});

                _realTimeInputs[name] = &_inputs[name];
            }
        else
            {
                // if the input is in the map, and onPress isnt the same as the previous value, assume that the
                // user wants to change the inverse function
                if (_inputs[name].onPress() != onPress)
                    {
                        _inputs[name].setInverseFunction(onInput);
                    }
            }
	}

template<typename T>
void inputManager<T>::add(const std::string &name, T key, std::function<void()> onInput, bool onPress, bool doubleClick, states activeState)
	{
		auto it = _inputs.find(name);
        if (it == _inputs.end())
            {
				_inputs[name] = input<T>(key, onInput, onPress, doubleClick, activeState);
                _inputs[name].setInverseFunction([] () {});

                _realTimeInputs[name] = &_inputs[name];
            }
        else
            {
                // if the input is in the map, and onPress isnt the same as the previous value, assume that the
                // user wants to change the inverse function
                if (_inputs[name].onPress() != onPress)
                    {
                        _inputs[name].setInverseFunction(onInput);
                    }
            }
	}

template<typename T>
void inputManager<T>::changeFunction(const std::string &name, std::function<void()> func)
    {
        if (_inputs.find(name) != _inputs.end())
            {
                _inputs[name].setFunction(func);
            }
        else
            {
                globals::_logger->log("Input \"" + name + "\" does not exist in input manager");
            }
    }

template<typename T>
void inputManager<T>::changeInverseFunction(const std::string &name, std::function<void()> func)
    {
        if (_inputs.find(name) != _inputs.end())
            {
                _inputs[name].setInverseFunction(func);
            }
        else
            {
                globals::_logger->log("Input \"" + name + "\" does not exist in input manager");
            }
    }

template<typename T>
sf::Time inputManager<T>::getTimeKeyHasBeenPressed(const std::string &name)
    {
        return _inputs[name].getTimePressed();
    }

template<typename T>
void inputManager<T>::remove(const std::string &name)
    {
        auto it = _inputs.find(name);
        if (it != _inputs.end())
            {
                _inputs.erase(it);
            }
        else
            {
                globals::_logger->log("Input \"" + name + "\" does not exist in input manager");
            }
    }

template<typename T>
void inputManager<T>::handleInput(states currentState)
    {
        for (auto &key : _realTimeInputs)
            {
                if (key.second)
                    {
                        if (key.second->getState() == states::ALL_STATES)
                            {
                                key.second->execute(states::ALL_STATES);
                            }
                        else
                            {
                                key.second->execute(currentState);
                            }
                    }
            }
    }

template<typename T>
void inputManager<T>::handleInput(sf::Event &event, states currentState)
    {
        for (auto &key : _eventTimeInputs)
            {
                if (key.second)
                    {   
                        if (key.second->getState() == states::ALL_STATES)
                            {
                                key.second->execute(event, states::ALL_STATES);
                            }
                        else
                            {
                                key.second->execute(event, currentState);
                            }
                    }
            }
    }
