#include "button.hpp"
#include "../game/globals.hpp"

#include <SFML/Graphics.hpp>

void button::_checkForMouse()
    {
        if (_app)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*_app);
                sf::Vector2f mousePosFloat(mousePos.x, mousePos.y);

                if (_bounds.contains(mousePosFloat))
                    {
                        _mouseOver = true;
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !_buttonClicked)
                            {
                                _buttonClicked = true;
                            }
                        else
                            {
                                _buttonClicked = false;
                            }
                    }
                else
                    {
                        _mouseOver = false;
                    }
            }
    }

button::button()
    {
        _buttonClicked = false;
        _mouseOver = false;

        _app = nullptr;
    }

void button::setWindow(sf::RenderWindow &app)
    {
        _app = &app;
    }

sf::Vector2f button::getPosition()
    {
        return sf::Vector2f(_bounds.left, _bounds.top);
    }

sf::FloatRect button::getBounds() const
    {
        return _bounds;
    }

void button::update()
    {

    }

button::~button()
    {}
