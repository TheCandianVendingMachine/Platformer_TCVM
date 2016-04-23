#include "countdown.hpp"

countdown::countdown()
    {
        _countdownTime = sf::Time::Zero;
        _clock.restart();
        _running = false;
    }

void countdown::start(sf::Time amount)
    {
        _countdownTime = amount;
        _running = true;
        _clock.restart();
    }

sf::Time countdown::getRemainingTime()
    {
        return sf::Time(_countdownTime - _clock.getElapsedTime());
    }

bool countdown::getRunning()
    {
        return _running;
    }

bool countdown::hasCountdownFinished()
    {
        if (_running)
            {
                if ((_countdownTime - _clock.getElapsedTime()).asMicroseconds() <= 0)
                    {
                        _running = false;
                        return true;
                    }

                return false;
            }
        
        return false;
    }
