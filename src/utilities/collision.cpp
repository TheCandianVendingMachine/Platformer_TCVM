#include "collision.hpp"
#include "../entity/entity.hpp"

std::pair<sf::Vector2f, sf::Vector2f> clsn::getDistance(entity *firstSprite, entity *secondSprite)
    {
        auto first = firstSprite->getSprite();
        auto second = secondSprite->getSprite();

        auto firstObjBound = first->getGlobalBounds();
        auto secondObjBound = second->getGlobalBounds();

        sf::Vector2f centerFirst(firstObjBound.left + (firstObjBound.width / 2),
                                 firstObjBound.top + (firstObjBound.height / 2));

        sf::Vector2f centerSecond(secondObjBound.left + (secondObjBound.width / 2),
                                  secondObjBound.top + (secondObjBound.height / 2));

        sf::Vector2f distance(centerFirst - centerSecond);
        sf::Vector2f minDistance((firstObjBound.width / 2) + (secondObjBound.width / 2),
                                 (firstObjBound.height / 2) + (secondObjBound.height / 2));

        return std::make_pair(distance, minDistance);
    }

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

bool clsn::hasCollided(entity *firstSprite, entity *secondSprite)
    {
        auto distance = clsn::getDistance(firstSprite, secondSprite);
        return abs(distance.first.x) < distance.second.x && abs(distance.first.y) < distance.second.y;
    }

bool clsn::hasCollided(sf::Vector2f posFirst, sf::Vector2f boundFirst, sf::Vector2f posSecond, sf::Vector2f boundSecond)
    {
        auto distance = clsn::getDistance(posFirst, boundFirst, posSecond, boundSecond);
        return abs(distance.first.x) < distance.second.x && abs(distance.first.y) < distance.second.y;
    }

sf::Vector2f clsn::getOverlap(entity *firstSprite, entity *secondSprite)
    {
        auto distance = clsn::getDistance(firstSprite, secondSprite);
        return sf::Vector2f(distance.first.x > 0 ? distance.second.x - distance.first.x : -distance.second.x - distance.first.x,
                            distance.first.y > 0 ? distance.second.y - distance.first.y : -distance.second.y - distance.first.y);
    }

bool clsn::handleCollision(entity *firstSprite, entity *secondSprite)
    {
        if (hasCollided(firstSprite, secondSprite))
            {
                sf::Vector2f overlap = getOverlap(firstSprite, secondSprite);
                if (abs(overlap.x) > abs(overlap.y))
                    {
                        // if Y is shallow axis, the entity will move to the bottom/top of the object
                        firstSprite->setPosition(firstSprite->getPosition().x, firstSprite->getPosition().y + overlap.y);
                    }
                else if (abs(overlap.x) < abs(overlap.y))
                    {
                        // if X is shallow axis, the entity will move to the side of the object
                        firstSprite->setPosition(firstSprite->getPosition().x + overlap.x, firstSprite->getPosition().y);
                    }
                else
                    {
                        // equal on both sides. Entity will move in diagonal direction
                       firstSprite->setPosition(firstSprite->getPosition().x + overlap.x, firstSprite->getPosition().y + overlap.y);
                    }

                return true;
            }

        return false;
    }
