// input.hpp
// input object. What will happen when key is pressed, and what state it will be active in
#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include <functional>

#include "../states/gameStates/states.hpp"

enum inputState
    {
        ON_PRESS,
        ON_RELEASE,
        REAL_TIME
    };

template<typename T>
class input
    {
        private:
            static constexpr sf::Event::EventType _release = (std::is_same<T, sf::Keyboard::Key>::value) ? sf::Event::KeyReleased : sf::Event::MouseButtonReleased;
            static constexpr sf::Event::EventType _press = (std::is_same<T, sf::Keyboard::Key>::value) ? sf::Event::KeyPressed : sf::Event::MouseButtonPressed;

            T _input;

            std::function<void()> _onInput;
            std::function<void()> _offInput;

            inputState _state;
            bool _onPress;

			bool _pressed;
            bool _pollRealtime;

			bool _doubleClick;

            states _activeState;

            sf::Clock _time;

        public:
            input() = default;
            input(T key, std::function<void()> onInput, inputState state, bool doubleClick, states activeState);
            input(T key, std::function<void()> onInput, bool onPress, bool doubleClick, states activeState);

            void execute(sf::Event &event, states active);
            void execute(states active);

            void setFunction(std::function<void()> func);

            const bool isRealTime() const;
            const bool onPress() const;

            void setInverseFunction(std::function<void()> func);

            sf::Time getTimePressed();

            T getInput() const;
            states getState() const;

    };

#include "input.inl"