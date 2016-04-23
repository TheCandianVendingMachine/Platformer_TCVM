// radioButton.hpp
// a toggleable button. Very simple
#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>

#include "button.hpp"

class checkBox : public button
    {
        protected:
            sf::Sprite _button;
            sf::Text _preButtonText;

            sf::Texture *_toggledTexture;
            sf::Texture *_nonToggledTexture;
            
            bool _toggle;
            bool _clicked;

        protected:
            void setButtonTexture();

        public:
            checkBox();

            void setTextures(sf::Texture *toggled, sf::Texture *nonToggled);
            void setPosition(sf::Vector2f pos);
            void setScale(float amount);
            
            void setSize(float X, float Y);
            void setSize(sf::Vector2f size);

            void setTextSize(float X, float Y);
            void setTextSize(sf::Vector2f size);

            void setColour(sf::Color colour);

            void setPreButtonString(const std::string &text);
            sf::Text *getText();

            void setToggled(bool value);
            const bool getToggled() const;

            sf::Sprite *getButton();

            void render(sf::RenderWindow &app);
            virtual void update();
    };