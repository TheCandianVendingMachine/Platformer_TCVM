// countdown.hpp
// a countdowner. Counts down when asked. Returns true when it has completed the countdown
#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class countdown
    {
        private:
            sf::Time _countdownTime;
            sf::Clock _clock;

            bool _running;

        public:
            countdown();

            void start(sf::Time amount);
            
            sf::Time getRemainingTime();
            bool getRunning();

            bool hasCountdownFinished();
    };