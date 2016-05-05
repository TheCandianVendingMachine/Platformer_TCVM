// state.hpp
// base class for a basic game state
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include <string>

#include "states.hpp"

class state
    {
        protected:
            states _state;

            bool _renderOvertop;
            bool _updateUnderneath;

            bool _isInitialized;

        public:
            state() = default;

            /*
            *  This is called to initialize anything that needs to be initialized once the
            *  state is pushed onto the stack
            *  The following should be initialized in the constructor
            *
            *  _renderOvertop
            *  _updateUnderneath
            *  _state
            *
            *  Everything else should be initialized in the initialize function
            */
            virtual void initialize() = 0;

            virtual void render() = 0;
            virtual void update(sf::Time deltaTime) = 0;

            const states getState() const;

            // Returns _renderOvertop
            const bool renderOvertop() const;
            // Returns _updateUnderneath
            const bool updateUnderneath() const;
            // Returns _isInitialized
            const bool isInitialized() const;

            // Cleans up the state. Called by constructor. Can be called manually
            virtual void cleanup() = 0;
            virtual ~state();
    };