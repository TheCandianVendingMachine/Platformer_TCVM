#include "quadtree.hpp"
#include "../gameObject/gameObject/gameObject.hpp"
#include "../gameObject/components/components.hpp"

#include <algorithm>
#include <iterator>

void quadtree::divide()
	{
		sf::Vector2f boundSize(_bounds.width, _bounds.height);
		sf::Vector2f newBoundSize(boundSize.x / 2, boundSize.y / 2);
		sf::Vector2f boundPos(_bounds.left, _bounds.top);

		sf::FloatRect newBoundsNW(boundPos, newBoundSize);
		sf::FloatRect newBoundsNE({boundPos.x + (boundSize.x / 2), boundSize.y}, newBoundSize);
		sf::FloatRect newBoundsSW({boundPos.x, boundPos.y + (boundSize.y / 2)}, newBoundSize);
		sf::FloatRect newBoundsSE(boundPos + newBoundSize, newBoundSize);

		_nWest = std::make_unique<quadtree>(quadtree(newBoundsNW));
		_nEast = std::make_unique<quadtree>(quadtree(newBoundsNE));
		_sWest = std::make_unique<quadtree>(quadtree(newBoundsSW));
		_sEast = std::make_unique<quadtree>(quadtree(newBoundsSE));
	}

quadtree::quadtree() : _maxGameObjects(4)
	{
	}

quadtree::quadtree(sf::FloatRect size) : _maxGameObjects(4)
	{
		_bounds = size;
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

				if (_objectsInNode.size() + 1 < _maxGameObjects)
					{
						_objectsInNode.push_back(obj);
						return true;
					}
				
				if (!_nWest.get()) 
					{
						divide();
					}

				if (_nWest->insert(obj)) return true;
				if (_nEast->insert(obj)) return true;
				if (_sWest->insert(obj)) return true;
				if (_sEast->insert(obj)) return true;

				return false;
			}
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
		std::merge(allObjects.begin(), allObjects.end(), obj.begin(), obj.end(), std::back_inserter(allObjects));

		obj = _nEast->getAllObjectsWithinRange(range);
		std::merge(allObjects.begin(), allObjects.end(), obj.begin(), obj.end(), std::back_inserter(allObjects));

		obj = _sWest->getAllObjectsWithinRange(range);
		std::merge(allObjects.begin(), allObjects.end(), obj.begin(), obj.end(), std::back_inserter(allObjects));

		obj = _sEast->getAllObjectsWithinRange(range);
		std::merge(allObjects.begin(), allObjects.end(), obj.begin(), obj.end(), std::back_inserter(allObjects));

		return allObjects;
	}
