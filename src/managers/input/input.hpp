// input.hpp
// input object. What will happen when key is pressed, and what state it will be active in
#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>

#include "../../states/states.hpp"

template<typename T>
class input
    {
        private:
            static constexpr sf::Event::EventType _release = (std::is_same<T, sf::Keyboard::Key>::value) ? sf::Event::KeyReleased : sf::Event::MouseButtonReleased;
            static constexpr sf::Event::EventType _press = (std::is_same<T, sf::Keyboard::Key>::value) ? sf::Event::KeyPressed : sf::Event::MouseButtonPressed;

            T _input;

            std::function<void()> _onInput;
            std::function<void()> _onOppositeInput;

            bool _onPress;

            states _activeState;

        public:
            input() = default;
            input(T key, std::function<void()> onInput, bool onPress, states activeState);
            void execute(sf::Event &event, states active);

            void setFunction(std::function<void()> func);
            void setInverseFunction(std::function<void()> func);

            T getInput() const;
            states getState() const;

    };

#include "input.inl"