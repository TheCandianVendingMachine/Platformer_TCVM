// quadtree.hpp
// a quad tree. Will allow for more shapes because objects only query those in range
#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <memory>

class gameObject;

class quadtree
	{
		private:
			std::vector<gameObject*> _objectsInNode;

			sf::FloatRect _bounds;

			std::unique_ptr<quadtree> _nWest;
			std::unique_ptr<quadtree> _nEast;
			std::unique_ptr<quadtree> _sWest;
			std::unique_ptr<quadtree> _sEast;

			const int _maxGameObjects;

			void divide();

		public:
			quadtree();
			quadtree(sf::FloatRect size);
			bool insert(gameObject *obj);
			std::vector<gameObject*> getAllObjectsWithinRange(sf::FloatRect range);

	};