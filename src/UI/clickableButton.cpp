#include "clickableButton.hpp"
#include "../game/globals.hpp"

clickableButton::clickableButton()
    {
        _textColour = sf::Color::White;
        _onButtonPress = [] () {};
    }

void clickableButton::setString(const std::string &buttonText)
    {
        _text.setString(buttonText);
        _bounds = _text.getGlobalBounds();
    }

void clickableButton::setFunction(std::function<void()> onButtonPress)
    {
        _onButtonPress = onButtonPress;
    }

void clickableButton::setColour(sf::Color colour)
    {
        _text.setColor(colour);
        _textColour = colour;
    }

void clickableButton::setSize(unsigned int size)
    {
        _text.setCharacterSize(size);
    }

void clickableButton::setPosition(sf::Vector2f pos)
    {
        _text.setPosition(pos);
        _bounds = _text.getGlobalBounds();
    }

sf::Text *clickableButton::getText()
    {
        return &_text;
    }

const std::string clickableButton::getString()
    {
        return _text.getString();
    }

void clickableButton::render(sf::RenderWindow &app)
    {
        app.draw(_text);
    }

void clickableButton::update()
    {
        _checkForMouse();
        if (_mouseOver)
            {
                _text.setColor(sf::Color::White);
            }
        else
            {
                _text.setColor(_textColour);
            }

        if (_buttonClicked)
            {
                _onButtonPress();
                _buttonClicked = false;
            }
    }
