// quadtree.hpp
// a quad tree. Will allow for more shapes because objects only query those in range
#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include <vector>

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

            std::shared_ptr<quadtree> _nWest;
            std::shared_ptr<quadtree> _nEast;
            std::shared_ptr<quadtree> _sWest;
            std::shared_ptr<quadtree> _sEast;

            quadtree *_parent;

            sf::FloatRect _bound;
            sf::RectangleShape _shape;

            bool _isBranch;
            unsigned int _level;

            const unsigned int _maxLevel;
            const unsigned int _maxGameObjects;

        private:
            void _divide();
            void _undivide();

        public:
            quadtree();
            quadtree(sf::FloatRect bounds, quadtree *parent = nullptr, unsigned int level = 0);

            void clear();

            void loadLevel(level &level);

            bool add(gameObject *obj);
            bool remove(gameObject *obj);

            void update(gameObject *obj);

            std::vector<gameObject*> getObjectsInRange(sf::FloatRect range);
            std::vector<gameObject*> getObjects();

            quadtree *getNode(gameObject *obj);
            quadtree *getNode(sf::Vector2f pos);

            unsigned int getAmountOfObjectsInNode();

            void draw(sf::RenderWindow &app);

            sf::RectangleShape *getOutline();
            unsigned int getLevel() const;

            quadtree &operator=(const quadtree &other);

	};