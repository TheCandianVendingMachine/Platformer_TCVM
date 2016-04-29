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
			float _deltaTime;

            std::vector<state*> _currentStates;
            std::queue<state*> _queuedState;

            bool _popState;
            int _amountToPop;

            bool _closeWindow;

            void popStateFromStack();

            // update the current state
            void update(sf::Time deltaTime);

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

            // handles input for the current tick
            void handleInput();
            // handle all state machine stuff, as well as updating the state
            void tick(sf::Time deltaTime);
            // render the current state
            void render();

            void closeWindow();
            sf::RenderWindow *getWindow() const;
            void setWindow(sf::RenderWindow *window);

			const float getDeltaTime() const;
			void setDeltaTime(float num);

            void cleanup();
            ~stateMachine();
    };