// inputManager.hpp
// manages inputs. Can manage mosue and keyboard input
#pragma once

#include <unordered_map>
#include <functional>

#include <SFML/System/Time.hpp>

#include "input.hpp"

template<typename T>
class inputManager
    {
        private:
            std::unordered_map<std::string, input<T>> _inputs;

            std::unordered_map<std::string, input<T>*> _eventTimeInputs;
            std::unordered_map<std::string, input<T>*> _realTimeInputs;

        public:
            // Any Non-Realtime inputs without function in constructor
            void add(const std::string &name, T key, inputState inputState, states activeState);
            // Any Non-Realtime inputs with function in constructor
            void add(const std::string &name, T key, std::function<void()> onInput, inputState inputState, states activeState);

            // Any Realtime inputs without function in constructor
            void add(const std::string &name, T key, bool onPress, states activeState);
            // Any Realtime inputs with function in constructor
            void add(const std::string &name, T key, std::function<void()> onInput, bool onPress, states activeState);


            void changeFunction(const std::string &name, std::function<void()> func);
            void changeInverseFunction(const std::string &name, std::function<void()> func);

            sf::Time getTimeKeyHasBeenPressed(const std::string &name);

            void remove(const std::string &name);

            void handleInput(states currentState);
            void handleInput(sf::Event &event, states currentState);
            

    };

#include "inputManager.inl"