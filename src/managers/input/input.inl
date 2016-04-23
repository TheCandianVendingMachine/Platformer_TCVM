#include "input.hpp"
template<typename T>
input<T>::input(T key, std::function<void()> onInput, bool onPress, states activeState)
    {
        _input = key;
        
        _onInput = onInput;
        _onPress = onPress;

        _activeState = activeState;
    }

template<typename T>
T input<T>::getInput() const
    {
        return _input;
    }

template<typename T>
void input<T>::execute(sf::Event &event, states active)
    {
        // if the active state is the current state, and the key is being pressed
        if (active == _activeState && event.key.code == _input && (_onPress ? event.type == _press : event.type == _release))
            {
                _onInput();
                _time.restart();
            }
        else if (active == _activeState && event.key.code == _input && (!_onPress ? event.type == _press : event.type == _release))
            {
                _onOppositeInput();
            }
    }

template<typename T>
void input<T>::setFunction(std::function<void()> func)
    {
        _onInput = func;
    }

template<typename T>
void input<T>::setInverseFunction(std::function<void()> func)
    {
        _onOppositeInput = func;
    }

template<typename T>
sf::Time input<T>::getTimePressed()
    {
        return _time.getElapsedTime();
    }

template<typename T>
states input<T>::getState() const
    {
        return _activeState;
    }