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
			sf::Vector2f _size;
            sf::Texture _texture;

        public:
            void setTexture(sf::Texture texture);
            sf::Texture *getTexture();

            sf::Sprite *getSprite();

			void setSize(float X, float Y);
			void setSize(sf::Vector2f size);

			sf::Vector2f getSize();

            void setPosition(float X, float Y);
            void setPosition(sf::Vector2f pos);

            sf::Vector2f getPosition();

            void setRotation(float factor);
            float getRotation();

            void draw(sf::RenderWindow &app);

    };