#include "input.hpp"
template<typename T>
input<T>::input(T key, std::function<void()> onInput, inputState state, states activeState)
    {
        _input = key;
        
        _onInput = onInput;
        _onPress = true;

        _activeState = activeState;
        _state = state;

        _pollRealtime = true;
    }

template<typename T>
input<T>::input(T key, std::function<void()> onInput, bool onPress, states activeState)
    {
        _input = key;
        
        _onInput = onInput;
        _onPress = onPress;

        _activeState = activeState;
        _state = inputState::REAL_TIME;

        _pollRealtime = true;
    }

template<typename T>
T input<T>::getInput() const
    {
        return _input;
    }

template<typename T>
void input<T>::execute(sf::Event &event, states active)
    {
        // this function should not be used for any real time inputs
        if (_state != REAL_TIME)
            {
                bool correctInput = (std::is_same<T, sf::Keyboard::Key>::value ? event.key.code : event.mouseButton.button) == _input;

                bool pressed = event.type == _press;
                bool released = event.type == _release;

                if (correctInput && active == _activeState)
                    {
                        if ((_state == ON_PRESS ? _press : _release) == event.type)
                            {
                                _onInput();
                                _time.restart();
                            }
                        else if ((_state != ON_PRESS ? _press : _release) == event.type)
                            {
                                _offInput();
                            }
                    }
            }
        else
            {
                globals::_logger->log("Current input state is real-time");
            }
    }

template<typename T>
void input<T>::execute(states active)
    {
        // this function should never be used for non-realtime inputs
        if (_state == REAL_TIME)
            {
                // static_cast is required. The compiler doesn't like mixing/matching types, even if it cannot happen, so we
                // cast it to the type it arleady is
                bool pressed = (std::is_same<T, sf::Keyboard::Key>::value ?
                                sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(_input)) :
                                sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(_input)));

                if (_pollRealtime && pressed == _onPress)
                    {
                        _onInput();
                        _time.restart();
                        _pollRealtime = true;
                    }
            }
        else
            {
                globals::_logger->log("Current input state is not real-time");
            }
    }

template<typename T>
void input<T>::setFunction(std::function<void()> func)
    {
        _onInput = func;
    }

template<typename T>
inline const bool input<T>::isRealTime() const
    {
        return _state == REAL_TIME;
    }

template<typename T>
inline const bool input<T>::onPress() const
    {
        return _onPress;
    }

template<typename T>
void input<T>::setInverseFunction(std::function<void()> func)
    {
        _offInput = func;
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