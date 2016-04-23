// scoreManager.hpp
// Manages all scores in the game
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <unordered_map>
#include <string>

#include "score.hpp"

class scoreManager
    {
        private:
            std::unordered_map<std::string, score> _scores;

        public:
            /* add new score to be tracked */
            void addNewScore(const std::string &name, sf::Vector2f pos);
            /* increment wanted score */
            void incrementScore(const std::string &name);
            /* reset the score */
            void resetScore(const std::string &name);

            /* return wanted score by pointer */
            score* getScore(const std::string &name);

            /* render all scores */
            void render(sf::RenderWindow &app);
    };