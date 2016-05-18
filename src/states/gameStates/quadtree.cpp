#include "quadtree.hpp"
#include "../gameObject/gameObject/gameObject.hpp"
#include "../gameObject/components/components.hpp"

#include <algorithm>
#include <iterator>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../utilities/logger/logger.hpp"
#include "level.hpp"

void quadtree::_merge(quadtree *node)
    {
        auto otherNodeObj = &node->_objectsInNode;
        for (auto it = otherNodeObj->begin(); it != otherNodeObj->end();)
            {
                _objectsInNode.push_back(*it);
                it = otherNodeObj->erase(it);
            }
    }

void quadtree::_undivide()
    {
        if (_nWest.get())
            {
                _nWest->_undivide();
                _merge(_nWest.get());
                _nWest = nullptr;

                _nEast->_undivide();
                _merge(_nEast.get());
                _nEast = nullptr;

                _sWest->_undivide();
                _merge(_sWest.get());
                _sWest = nullptr;

                _sEast->_undivide();
                _merge(_sEast.get());
                _sEast = nullptr;
            }
    }

void quadtree::_divide()
	{
		sf::Vector2f boundSize(_bounds.width, _bounds.height);
		sf::Vector2f newBoundSize(boundSize.x / 2, boundSize.y / 2);
		sf::Vector2f boundPos(_bounds.left, _bounds.top);

		sf::FloatRect newBoundsNW(boundPos, newBoundSize);
		sf::FloatRect newBoundsNE({boundPos.x + newBoundSize.x, boundPos.y}, newBoundSize);
		sf::FloatRect newBoundsSW({boundPos.x, boundPos.y + newBoundSize.y}, newBoundSize);
		sf::FloatRect newBoundsSE(boundPos + newBoundSize, newBoundSize);
        
		_nWest = std::make_shared<quadtree>(quadtree(newBoundsNW));
        _nWest->_parent = this;

		_nEast = std::make_shared<quadtree>(quadtree(newBoundsNE));
        _nEast->_parent = this;

		_sWest = std::make_shared<quadtree>(quadtree(newBoundsSW));
        _sWest->_parent = this;

		_sEast = std::make_shared<quadtree>(quadtree(newBoundsSE));
        _sEast->_parent = this;

        for (auto it = _objectsInNode.begin(); it != _objectsInNode.end();)
            {
                _nWest->insert(*it);
                _nEast->insert(*it);
                _sWest->insert(*it);
                _sEast->insert(*it);
                it = _objectsInNode.erase(it);
            }
	}

quadtree::quadtree() : _maxGameObjects(3)
	{
        _outline.setSize({ 0, 0 });
	}

quadtree::quadtree(sf::FloatRect size) : _maxGameObjects(3)
	{
        setBounds(size);
	}

void quadtree::setBounds(sf::FloatRect size)
    {
        _bounds = size;
        _outline.setSize({ size.width - 5, size.height - 5 });
        _outline.setPosition(_bounds.left, _bounds.top);
        _outline.setFillColor(sf::Color::Transparent);
        _outline.setOutlineThickness(5);
        _outline.setOutlineColor(sf::Color(153, 255, 12, 170));
    }

void quadtree::loadLevelIntoTree(level &lvl)
    {
        clear();

        for (auto &entName : *lvl.getAllGameObjects())
            {
                for (auto &ent : entName.second)
                    {
                        insert(ent);
                    }
            }
    }

void quadtree::update(gameObject *obj)
    {
        /*remove(obj);
        insert(obj);*/
    }

bool quadtree::remove(gameObject *obj)
    {
        auto it = std::remove_if(_objectsInNode.begin(), _objectsInNode.end(), [obj] (gameObject *objTest) { return objTest->getID() == obj->getID(); });
        if (it != _objectsInNode.end())
            {
                _objectsInNode.erase(it);
                if (getObjectsInTree() <= _maxGameObjects && getObjectsInTree() >= 0)
                    {
                        _undivide();
                    }
                return true;
            }

        if (!_nWest.get())
            {
                return false;
            }

        if (_nWest->remove(obj)) return true;
        if (_nEast->remove(obj)) return true;
        if (_sWest->remove(obj)) return true;
        if (_sEast->remove(obj)) return true;

        return false;
    }

bool quadtree::insert(gameObject *obj)
	{
		auto tc = obj->get<textureComponent>();
		if (tc) 
			{
				auto pos = tc->getPosition();

				if (!_bounds.contains(pos))
					{
						return false;
					}

				if (_objectsInNode.size() < _maxGameObjects)
					{
						_objectsInNode.push_back(obj);
						return true;
					}

				if (!_nWest.get()) 
					{
						_divide();
					}

				if (_nWest->insert(obj)) return true;
				if (_nEast->insert(obj)) return true;
				if (_sWest->insert(obj)) return true;
				if (_sEast->insert(obj)) return true;

                globals::_logger->log("Cannot add \"" + obj->getName() + "\" into tree");
				return false;
			}

        return false;
	}

void quadtree::clear()
    {
        _objectsInNode.clear();
        if (!_nWest.get())
            {
                // if theres no north-west node, the tree didnt split up
                // we can assume that there are no nodes other than the current
                return;
            }
        _nWest->clear();
        _nWest = nullptr;

        _nEast->clear();
        _nEast = nullptr;

        _sWest->clear();
        _sWest = nullptr;

        _sEast->clear();
        _sEast = nullptr;
    }

void quadtree::draw(sf::RenderWindow &app)
    {
        app.draw(_outline);

        if (!_nWest.get())
            {
                return;
            }

        _nWest->draw(app);
        _nEast->draw(app);
        _sWest->draw(app);
        _sEast->draw(app);
    }

std::vector<gameObject*> quadtree::getAllObjectsWithinRange(sf::FloatRect range)
	{
		std::vector<gameObject*> allObjects;
		
		if (!_bounds.intersects(range))
			{
				return allObjects;
			}

		for (auto &obj : _objectsInNode)
			{
				auto cc = obj->get<collisionComponent>();
				if (cc) 
					{
						auto bounds = cc->getBounds();
						if (range.intersects(bounds))
							{
								allObjects.push_back(obj);
							}
					}
			}

		if (!_nWest.get())
			{
				return allObjects;
			}
        
		auto obj = _nWest->getAllObjectsWithinRange(range);
		std::copy(obj.begin(), obj.end(), std::back_inserter(allObjects));

		obj = _nEast->getAllObjectsWithinRange(range);
        std::copy(obj.begin(), obj.end(), std::back_inserter(allObjects));

		obj = _sWest->getAllObjectsWithinRange(range);
        std::copy(obj.begin(), obj.end(), std::back_inserter(allObjects));

		obj = _sEast->getAllObjectsWithinRange(range);
        std::copy(obj.begin(), obj.end(), std::back_inserter(allObjects));

		return allObjects;
	}

int quadtree::getObjectsInTree()
    {
        unsigned int amount = _objectsInNode.size();

        if (!_nWest)
            {
                return amount;
            }

        amount += _nWest->getObjectsInTree();
        amount += _nEast->getObjectsInTree();
        amount += _sWest->getObjectsInTree();
        amount += _sEast->getObjectsInTree();

        return amount;
    }

quadtree &quadtree::operator=(const quadtree &other)
    {
        if (this != &other)
            {
                _bounds = other._bounds;
                _objectsInNode = other._objectsInNode;
            }
        return *this;
    }
