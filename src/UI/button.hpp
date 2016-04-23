// button.hpp
// basic button class. Has a std::function that executes when button is pressed
#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <functional>

class button
    {
        protected:
            sf::RenderWindow *_app;
            sf::FloatRect _bounds;

            bool _buttonClicked;
            bool _mouseOver;

        protected:
            void _checkForMouse();

        public:
            button();
            
            void setWindow(sf::RenderWindow &app);

            sf::Vector2f getPosition();
            sf::FloatRect getBounds() const;

            virtual void update() = 0;
            virtual void render(sf::RenderWindow &app) = 0;

            virtual ~button();

    };