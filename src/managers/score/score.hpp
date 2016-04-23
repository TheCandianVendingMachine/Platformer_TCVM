// score.hpp
// a way to simply add scores to an object. Manages the text so it can be rendered
#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class score
    {
        private:
            sf::Text _text;

            int _score;
            int _incrementAmount;

        public:
            score();

            void setIncrementAmount(int amount);
            const int getIncrementAmount() const;

            void increment();

            void setScore(int amount);

            int getScore() const;
            const std::string getScoreAsString();

            sf::Text &getText();

            void render(sf::RenderWindow &app);
    };