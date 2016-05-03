// collisionComponent.hpp
// allows entities to have a bounding box
#pragma once

#include "component.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

class gameObject;

class collisionComponent : public component
    {
        private:
            sf::FloatRect _boundingBox;
            sf::Vector2f _positionOffset;

            std::string _onCollide;

            bool _colliding;

            std::pair<sf::Vector2f, sf::Vector2f> _getDistance(collisionComponent &other);
            sf::Vector2f _getOverlap(collisionComponent &other);

        public:
            collisionComponent();
            void setOnCollisionScript(const std::string &collisionScript);

            void setBounds(sf::Vector2f size, sf::Vector2f offset);

            void update();
            sf::FloatRect &getBounds();

            bool getColliding();

            bool hasCollided(collisionComponent &other);
            bool collide(gameObject *other);

    };