#include "checkBox.hpp"
#include "../game/globals.hpp"

void checkBox::setButtonTexture()
    {
        if (_toggle)
            {
                _button.setTexture(*_toggledTexture);
            }
        else
            {
                _button.setTexture(*_nonToggledTexture);
            }
    }

checkBox::checkBox()
    {
        _toggle = false;

        _nonToggledTexture = nullptr;
        _toggledTexture = nullptr;

    }

void checkBox::setTextures(sf::Texture *toggled, sf::Texture *nonToggled)
    {
        _toggledTexture = toggled;
        _nonToggledTexture = nonToggled;

        setButtonTexture();
    }

void checkBox::setPosition(sf::Vector2f pos)
    {
        _preButtonText.setPosition(pos);
        _button.setPosition(_preButtonText.getPosition() + sf::Vector2f(_preButtonText.getLocalBounds().width, 5));
        _bounds = _button.getGlobalBounds();
    }

void checkBox::setScale(float amount)
    {
        _button.scale(amount, amount);
        _bounds = _button.getGlobalBounds();
    }

void checkBox::setSize(float X, float Y)
    {
        _button.scale(X / _button.getGlobalBounds().width, Y / _button.getGlobalBounds().height);
        _bounds = _button.getGlobalBounds();
    }

void checkBox::setSize(sf::Vector2f size)
    {
        setSize(size.x, size.y);
    }

void checkBox::setTextSize(float X, float Y)
    {
        _preButtonText.scale(X / _preButtonText.getGlobalBounds().width, Y / _button.getGlobalBounds().height);
        setPosition(_preButtonText.getPosition());
    }

void checkBox::setTextSize(sf::Vector2f size)
    {
        setTextSize(size.x, size.y);
    }

void checkBox::setColour(sf::Color colour)
    {
        _preButtonText.setColor(colour);
        _button.setColor(colour);
    }

void checkBox::setPreButtonString(const std::string &text)
    {
        _preButtonText.setString(text);
        setPosition(_button.getPosition());
    }

sf::Text *checkBox::getText()
    {
        return &_preButtonText;
    }

void checkBox::setToggled(bool value)
    {
        _toggle = value;
        setButtonTexture();
    }

const bool checkBox::getToggled() const
    {
        return _toggle;
    }

sf::Sprite *checkBox::getButton()
    {
        return &_button;
    }

void checkBox::render(sf::RenderWindow &app)
    {
        app.draw(_button);
        app.draw(_preButtonText);
    }

void checkBox::update()
    {
        _checkForMouse();
        if (_buttonClicked && !_clicked)
            {
                _toggle = !_toggle;
                _clicked = true;
                setButtonTexture();
            }
        else if (!_buttonClicked && _clicked)
            {
                _clicked = false;
            }

    }
