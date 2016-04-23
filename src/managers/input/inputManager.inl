#include "inputManager.hpp"
template<typename T>
inline void inputManager<T>::add(const std::string &name, T key, bool onPress, states activeState)
    {
        _inputs[name] = input<T>(key, [] () {}, onPress, activeState);
        _inputs[name].setInverseFunction([] () {});
    }

template<typename T>
void inputManager<T>::add(const std::string &name, T key, std::function<void()> onInput, bool onPress, states activeState)
    {
        _inputs[name] = input<T>(key, onInput, onPress, activeState);
        _inputs[name].setInverseFunction([] () {});
    }

template<typename T>
void inputManager<T>::changeFunction(const std::string &name, std::function<void()> func)
    {
        _inputs[name].setFunction(func);
    }

template<typename T>
inline void inputManager<T>::changeInverseFunction(const std::string & name, std::function<void()> func)
    {
        _inputs[name].setInverseFunction(func);
    }

template<typename T>
void inputManager<T>::remove(const std::string &name)
    {
        auto it = _inputs.find(name);
        if (it != _inputs.end())
            {
                _inputs.erase(it);
            }
    }

template<typename T>
void inputManager<T>::handleInput(sf::Event &event, states currentState)
    {
        for (auto &key : _inputs)
            {
                key.second.execute(event, currentState);
            }
    }
