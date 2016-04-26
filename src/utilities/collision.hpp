// collision.hpp
// collision checkers and handler
#pragma once

#include <SFML/System/Vector2.hpp>
#include <utility>

namespace clsn
    {
        std::pair<sf::Vector2f, sf::Vector2f> getDistance(sf::Vector2f posFirst, sf::Vector2f boundFirst, sf::Vector2f posSecond, sf::Vector2f boundSecond);
        bool hasCollided(sf::Vector2f posFirst, sf::Vector2f boundFirst, sf::Vector2f posSecond, sf::Vector2f boundSecond);
    }