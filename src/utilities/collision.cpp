#include "collision.hpp"

std::pair<sf::Vector2f, sf::Vector2f> clsn::getDistance(sf::Vector2f posFirst, sf::Vector2f boundFirst, sf::Vector2f posSecond, sf::Vector2f boundSecond)
    {
        sf::Vector2f centerFirst(posFirst.x + (boundFirst.x / 2),
                                 posFirst.y + (boundFirst.y / 2));

        sf::Vector2f centerSecond(posSecond.x + (boundSecond.x / 2),
                                  posSecond.y + (boundSecond.y / 2));

        sf::Vector2f distance(centerFirst - centerSecond);
        sf::Vector2f minDistance((boundFirst.x / 2) + (boundSecond.x / 2),
                                 (boundFirst.y / 2) + (boundSecond.y / 2));

        return std::make_pair(distance, minDistance);
    }

bool clsn::hasCollided(sf::Vector2f posFirst, sf::Vector2f boundFirst, sf::Vector2f posSecond, sf::Vector2f boundSecond)
    {
        auto distance = clsn::getDistance(posFirst, boundFirst, posSecond, boundSecond);
        return (abs(static_cast<int>(distance.first.x)) < distance.second.x && abs(static_cast<int>(distance.first.y)) < distance.second.y);
    }