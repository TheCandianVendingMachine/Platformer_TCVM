#include "score.hpp"
#include "../../game/globals.hpp"

score::score()
    {
        _score = 0;
        _incrementAmount = 1;

        _text.setFont(*globals::_fontManager->get("gameFont", false));
        _text.setString(std::to_string(_score));
    }

void score::setIncrementAmount(int amount)
    {
        _incrementAmount = amount;
    }

const int score::getIncrementAmount() const
    {
        return _incrementAmount;
    }

void score::increment()
    {
        _score += _incrementAmount;
        _text.setString(std::to_string(_score));
    }

void score::setScore(int amount)
    {
        _score = amount;
    }

int score::getScore() const
    {
        return _score;
    }

const std::string score::getScoreAsString()
    {
        return _text.getString();
    }

sf::Text &score::getText()
    {
        return _text;
    }

void score::render(sf::RenderWindow &app)
    {
        app.draw(_text);
    }
