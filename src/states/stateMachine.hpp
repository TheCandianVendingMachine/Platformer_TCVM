// stateMachine.hpp
// handles all states in the game. Can push new states to the queue and pop them off as needed
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include <queue>

class state;

class stateMachine
    {
        private:
            sf::RenderWindow *_window;

            std::vector<state*> _currentStates;
            std::queue<state*> _queuedState;

            // handles input for the current tick
            void handleInput();
            // update the current state
            void update(sf::Time deltaTime);

            bool _popState;
            int _amountToPop;

            bool _closeWindow;

            void popStateFromStack();

        public:
            stateMachine() = default;
            stateMachine(sf::RenderWindow *window);

            void queueState(state *newState);
            void pushState(state *newState);
            void popState();
            void popAllStates();
            void reinitState();

            state* getCurrentState() const;
            std::vector<state*> *getAllStates();
            
            state* getStateUnderneath();

            // handles current tick
            void tick(sf::Time deltaTime);
            // render the current state
            void render();

            void closeWindow();
            sf::RenderWindow *getWindow() const;
            void setWindow(sf::RenderWindow *window);

            void cleanup();
            ~stateMachine();
    };