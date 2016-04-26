#include "stringInputManager.hpp"
#include "../../game/globals.hpp"

void stringInputManager::clearString()
    {
        _textEntered.clear();
    }

void stringInputManager::getInput(sf::Event &event)
    {
        if (event.type == sf::Event::TextEntered)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !_textEntered.isEmpty())
                    {
                        _textEntered.erase(_textEntered.getSize() - 1, 1);
                    }
                else if (event.text.unicode < 128)
                    {
                        _textEntered += event.text.unicode;
                    }
            }
    }

sf::String &stringInputManager::getTextEntered()
    {
        return _textEntered;
    }
