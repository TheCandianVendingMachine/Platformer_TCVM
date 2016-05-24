#include "quadtree.hpp"
#include "level.hpp"

#include "../gameObject/gameObject/gameObject.hpp"
#include "../gameObject/components/components.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iterator>

void quadtree::_divide()
    {
        sf::FloatRect newBounds(_bound.left, _bound.top,
                                _bound.width / 2.f, _bound.height / 2.f);
        _nWest = std::make_shared<quadtree>(quadtree(newBounds, this, _level + 1));

        newBounds = { _bound.left + (_bound.width / 2.f), _bound.top,
                      _bound.width / 2.f, _bound.height / 2.f};
        _nEast = std::make_shared<quadtree>(quadtree(newBounds, this, _level + 1));

        newBounds = { _bound.left , _bound.top + (_bound.height / 2.f),
                      _bound.width / 2.f, _bound.height / 2.f };
        _sWest = std::make_shared<quadtree>(quadtree(newBounds, this, _level + 1));

        newBounds = { _bound.left + (_bound.width / 2.f), _bound.top + (_bound.height / 2.f),
                      _bound.width / 2.f, _bound.height / 2.f };
        _sEast = std::make_shared<quadtree>(quadtree(newBounds, this, _level + 1));

        for (auto &obj : _objectsInNode)
            {
                add(obj);
            }
        _objectsInNode.clear();
    }

void quadtree::_undivide()
    {
        _objectsInNode.insert(_objectsInNode.end(), _nWest->_objectsInNode.begin(), _nWest->_objectsInNode.end());
        _nWest->clear();
        _nWest = nullptr;

        _objectsInNode.insert(_objectsInNode.end(), _nEast->_objectsInNode.begin(), _nEast->_objectsInNode.end());
        _nEast->clear();
        _nEast = nullptr;

        _objectsInNode.insert(_objectsInNode.end(), _sWest->_objectsInNode.begin(), _sWest->_objectsInNode.end());
        _sWest->clear();
        _sWest = nullptr;

        _objectsInNode.insert(_objectsInNode.end(), _sEast->_objectsInNode.begin(), _sEast->_objectsInNode.end());
        _sEast->clear();
        _sEast = nullptr;
    }

quadtree::quadtree() : _maxGameObjects(3), _maxLevel(7)
    {
        _bound = sf::FloatRect();
        _isBranch = false;
    }

quadtree::quadtree(sf::FloatRect bounds, quadtree *parent, unsigned int level) : _maxGameObjects(3), _maxLevel(7)
    {
        _parent = parent;

        _level = level;
        
        _bound = bounds;
        _shape.setSize(sf::Vector2f(_bound.width - 5, _bound.height - 5));
        _shape.setPosition(_bound.left, _bound.top);

        _shape.setFillColor(sf::Color::Transparent);

        _shape.setOutlineThickness(5);
        _shape.setOutlineColor(sf::Color(144, 40, 40, 200));

        _isBranch = false;
    }

void quadtree::clear()
    {
        if (_nWest)
            {
                _nWest->clear();
                _nWest = nullptr;

                _nEast->clear();
                _nEast = nullptr;

                _sWest->clear();
                _sWest = nullptr;

                _sEast->clear();
                _sEast = nullptr;
            }

        _objectsInNode.clear();
        _isBranch = false;
    }

void quadtree::loadLevel(level &level)
    {
        auto allGameObjects = level.getAllGameObjects();
        for (auto &objIndex : *allGameObjects)
            {
                for (auto &obj : objIndex.second)
                    {
                        add(obj);
                    }
            }
    }

bool quadtree::add(gameObject *obj)
    {
        auto tc = obj->get<textureComponent>();
        if (tc)
            {
                auto pos = tc->getPosition();
                auto size = tc->getSize();
                if (_bound.contains(pos))
                    {
                        if ((_objectsInNode.size() < _maxGameObjects && !_isBranch) || !(_level != _maxLevel))
                            {
                                _objectsInNode.push_back(obj);
                                return true;
                            }

                        if (!_nWest)
                            {
                                _isBranch = true;
                                _divide();
                            }

                        bool added = false;
                        added |= _nWest->add(obj);
                        added |= _nEast->add(obj);
                        added |= _sWest->add(obj);
                        added |= _sEast->add(obj);
                        return added;
                    }
            }

        return false;
    }

bool quadtree::remove(gameObject *obj)
    {
        auto it = std::remove_if(_objectsInNode.begin(), _objectsInNode.end(), [obj] (gameObject *testObj) { return testObj->getID() == obj->getID(); });
        if (it != _objectsInNode.end())
            {
                _objectsInNode.erase(it);
                if (_parent)
                    {
                        if (_parent->getAmountOfObjectsInNode() <= _maxGameObjects)
                            {
                                _parent->_isBranch = false;
                                _parent->_undivide();
                            }
                    }
                return true;
            }

        if (_nWest)
            {
                if (_nWest->remove(obj)) return true;
                if (_nEast->remove(obj)) return true;
                if (_sWest->remove(obj)) return true;
                if (_sEast->remove(obj)) return true;
            }

        return false;
    }

void quadtree::update(gameObject *obj)
    {
        auto node = getNode(obj);
        if (node)
            {
                if (node->_parent)
                    {
                        node->remove(obj);
                        if (!node->_parent->add(obj))
                            {
                                add(obj);
                            }
                    }
            }
    }

std::vector<gameObject*> quadtree::getObjectsInRange(sf::FloatRect range)
    {
        if (_bound.intersects(range))
            {
                if (_nWest)
                    {
                        std::vector<gameObject*> retVec;

                        auto objects = _nWest->getObjectsInRange(range);
                        retVec.insert(retVec.end(), objects.begin(), objects.end());

                        objects = _nEast->getObjectsInRange(range);
                        retVec.insert(retVec.end(), objects.begin(), objects.end());

                        objects = _sWest->getObjectsInRange(range);
                        retVec.insert(retVec.end(), objects.begin(), objects.end());

                        objects = _sEast->getObjectsInRange(range);
                        retVec.insert(retVec.end(), objects.begin(), objects.end());

                        return retVec;
                    }

                return _objectsInNode;
            }

        return std::vector<gameObject*>();
    }

std::vector<gameObject*> quadtree::getObjects()
    {
        return _objectsInNode;
    }

quadtree *quadtree::getNode(gameObject *obj)
    {
        auto it = std::find_if(_objectsInNode.begin(), _objectsInNode.end(), [obj] (gameObject *testObj) { return testObj->getID() == obj->getID(); });
        if (it != _objectsInNode.end())
            {
                return this;
            }

        if (_nWest.get())
            {
                auto node = _nWest->getNode(obj);
                if (node) return node;

                node = _nEast->getNode(obj);
                if (node) return node;

                node = _sWest->getNode(obj);
                if (node) return node;

                node = _sEast->getNode(obj);
                if (node) return node;

                return nullptr;
            }

        return nullptr;
    }

quadtree *quadtree::getNode(sf::Vector2f pos)
    {
        if (_bound.contains(pos))
            {
                if (_nWest)
                    {
                        auto node = _nWest->getNode(pos);
                        if (node) return node;

                        node = _nEast->getNode(pos);
                        if (node) return node;

                        node = _sWest->getNode(pos);
                        if (node) return node;

                        node = _sEast->getNode(pos);
                        if (node) return node;

                        return nullptr;
                    }
                return this;
            }

        return nullptr;
    }

unsigned int quadtree::getAmountOfObjectsInNode()
    {
        unsigned int amount = 0;
        if (_isBranch)
            {
                amount += _nWest->getAmountOfObjectsInNode();
                amount += _nEast->getAmountOfObjectsInNode();
                amount += _sWest->getAmountOfObjectsInNode();
                amount += _sEast->getAmountOfObjectsInNode();
            }
        else
            {
                return _objectsInNode.size();
            }
        return amount;
    }

void quadtree::draw(sf::RenderWindow &app)
    {
        if (!_isBranch)
            {
                app.draw(_shape);
            }

        if (_nWest)
            {
                _nWest->draw(app);
                _nEast->draw(app);
                _sWest->draw(app);
                _sEast->draw(app);
            }
    }

sf::RectangleShape *quadtree::getOutline()
    {
        return &_shape;
    }

unsigned int quadtree::getLevel() const
    {
        return _level;
    }

quadtree &quadtree::operator=(const quadtree &other)
    {
        if (&other != this)
            {
                _level = other._level;

                _bound = other._bound;
                _shape = other._shape;
                _isBranch = other._isBranch;

                _nWest = other._nWest;
                _nEast = other._nEast;
                _sWest = other._sWest;
                _sEast = other._sEast;
            }

        return *this;
    }
