// cameraComponent.hpp
// sets up a camera.
#pragma once

#include "component.hpp"
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class gameObject;
namespace sf
	{
		class RenderWindow;
	}

class cameraComponent : public component
	{
		private:
			sf::RenderWindow *_app;

			sf::View _view;
			sf::Vector2f _impulse;
			bool _follow;

		public:
			void setWindow(sf::RenderWindow *app);

			void setCameraSize(sf::Vector2f size);
			void setCameraPos(sf::Vector2f pos);

			void setFollow(bool follow);

			void update(sf::Time deltaTime);

	};