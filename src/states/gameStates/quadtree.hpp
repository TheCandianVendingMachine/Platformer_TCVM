// quadtree.hpp
// a quad tree. Will allow for more shapes because objects only query those in range
#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>

class gameObject;
class level;

namespace sf
    {
        class RenderWindow;
    }

class quadtree
	{
		private:
			std::vector<gameObject*> _objectsInNode;

			sf::FloatRect _bounds;
            sf::RectangleShape _outline;

			std::shared_ptr<quadtree> _nWest;
			std::shared_ptr<quadtree> _nEast;
			std::shared_ptr<quadtree> _sWest;
			std::shared_ptr<quadtree> _sEast;

            quadtree *_parent;

			const int _maxGameObjects;

            void _merge(quadtree *node);

            void _undivide();
			void _divide();

		public:
			quadtree();
			quadtree(sf::FloatRect size);

            void setBounds(sf::FloatRect size);
            // loads an initialized level into the quadtree, and adds all entities into tree
            void loadLevelIntoTree(level &lvl);

            void update(gameObject *obj);

            bool remove(gameObject *obj);
			bool insert(gameObject *obj);

            void clear();

            void draw(sf::RenderWindow &app);

			std::vector<gameObject*> getAllObjectsWithinRange(sf::FloatRect range);
            int getObjectsInTree();

            quadtree &operator=(const quadtree &other);

	};