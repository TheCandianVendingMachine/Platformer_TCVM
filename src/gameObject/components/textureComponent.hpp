// textureComponent.hpp
// the texture component. Has a sf::Texture pointer, and the sprite that holds it
#pragma once

#include "component.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class textureComponent : public component
    {
        private:
            sf::Sprite _sprite;
            sf::Texture _texture;

        public:
            void setTexture(sf::Texture texture);
            sf::Texture *getTexture();

            sf::Sprite *getSprite();

            void setPosition(float X, float Y);
            void setPosition(sf::Vector2f pos);

            void draw(sf::RenderWindow &app);

    };