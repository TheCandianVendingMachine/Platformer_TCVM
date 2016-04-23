// collision.hpp
// collision checkers and handler
#pragma once

#include <SFML/System/Vector2.hpp>
#include <utility>

class entity;

namespace clsn
    {
        std::pair<sf::Vector2f, sf::Vector2f> getDistance(entity *firstSprite, entity *secondSprite);
        std::pair<sf::Vector2f, sf::Vector2f> getDistance(sf::Vector2f posFirst, sf::Vector2f boundFirst, sf::Vector2f posSecond, sf::Vector2f boundSecond);

        bool hasCollided(entity *firstSprite, entity *secondSprite);
        bool hasCollided(sf::Vector2f posFirst, sf::Vector2f boundFirst, sf::Vector2f posSecond, sf::Vector2f boundSecond);

        sf::Vector2f getOverlap(entity *firstSprite, entity *secondSprite);

        bool handleCollision(entity *firstSprite, entity *secondSprite);
    }