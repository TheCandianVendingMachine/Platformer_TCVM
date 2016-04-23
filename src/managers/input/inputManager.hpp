// inputManager.hpp
// manages inputs. Can manage mosue and keyboard input
#pragma once

#include <unordered_map>
#include <functional>

#include <SFML/Window/Keyboard.hpp>

#include "input.hpp"

template<typename T>
class inputManager
    {
        private:
            std::unordered_map<std::string, input<T>> _inputs;

        public:
            void add(const std::string &name, T key, bool onPress, states activeState);
            void add(const std::string &name, T key, std::function<void()> onInput, bool onPress, states activeState);

            void changeFunction(const std::string &name, std::function<void()> func);
            void changeInverseFunction(const std::string &name, std::function<void()> func);

            void remove(const std::string &name);

            void handleInput(sf::Event &event, states currentState);

    };

#include "inputManager.inl"