#include "level.hpp"

#include "../gameObject/gameObject/gameObject.hpp"
#include "../utilities/collision.hpp"
#include "../gameObject/components/components.hpp"

#include "../utilities/strFuncs.hpp"
#include "../game/globals.hpp"

#include "stateMachine.hpp"

#include "../utilities/loadJsonFile.hpp"

#include "../utilities/logger/logger.hpp"

level::level()
    {
        _factory.initializeJsonFile("assets/entities/default_entity_list.json");
        _quadtree = quadtree(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(3000, 1500)));
    }

void level::unloadLevel()
    {
        _entities.clear();
        _factory.clear();
        _quadtree.clear();
    }

void level::load(const std::string &levelPath)
    {
        unloadLevel();

        if (!levelPath.empty())
            {
                Json::Value root;
                std::ifstream read(levelPath);
                if (read.is_open())
                    {
                        read >> root;
                    }
                read.close();

                auto members = root["level_entity_lists"].getMemberNames();
                for (auto &entLists : members)
                    {
                        _factory.initializeJsonFile(entLists);
                    }

                sf::Vector2f displacementAmount(0, 0);
                bool displacement = false;

                members = root["level"]["entity"].getMemberNames();
                for (auto &ent : members)
                    {
                        auto vars = root["level"]["entity"][ent];

                        std::string entName = strfn::splitString(ent, ':').first;
                        auto obj = addEntity(entName);

                        auto textureComp = obj->get<textureComponent>();
                        if (textureComp)
                            {
                                sf::Vector2f pos(vars["position"]["X"].asFloat(), vars["position"]["Y"].asFloat());
                                if (pos.x < 0)
                                    {
                                        displacementAmount.x = abs(0 - pos.x);
                                        displacement = true;
                                    }
                                if (pos.y < 0)
                                    {
                                        displacementAmount.y = abs(0 - pos.y);
                                        displacement = true;
                                    }

                                textureComp->setPosition(pos);
                                textureComp->setSize(vars["size"]["X"].asFloat(), vars["size"]["Y"].asFloat());
                                textureComp->setRotation(vars["rotation"].asFloat());
                            }
                    }

                if (displacement)
                    {
                        for (auto &ent : _entities)
                            {
                                auto tc = ent->get<textureComponent>();
                                if (tc)
                                    {
                                        tc->setPosition(tc->getPosition() + displacementAmount);
                                    }
                            }
                    }

                globals::_logger->logToConsole("Level Loaded");
            }

        _quadtree.clear();
        _quadtree.loadLevel(*this);
    }

void level::save(const std::string &levelPath)
    {
        if (!levelPath.empty())
            {
                unsigned int entCount = 0;
                Json::Value root;
                root["level"]["entity"];
                for (auto &ent : _entities)
                    {
                        sf::Vector2f pos(0, 0);
                        sf::Vector2f size(0, 0);
                        float angle = 0.f;

                        auto textureComp = ent->get<textureComponent>();
                        if (textureComp)
                            {
                                pos = textureComp->getPosition();
                                size = textureComp->getSize();
                                angle = textureComp->getRotation();
                            }

                        std::string entName = ent->getName() + ":" + std::to_string(++entCount);

                        root["level"]["entity"][entName]["position"]["X"] = pos.x;
                        root["level"]["entity"][entName]["position"]["Y"] = pos.y;
                        root["level"]["entity"][entName]["size"]["X"] = size.x;
                        root["level"]["entity"][entName]["size"]["Y"] = size.y;
                        root["level"]["entity"][entName]["rotation"] = angle;
                    }

                Json::Value loadedRoot;
                ljf::loadJsonFile(levelPath, &loadedRoot);
                root["level_entity_lists"];
                auto members = loadedRoot["level_entity_lists"].getMemberNames();
                for (auto &entLists : members)
                    {
                        root["level_entity_lists"][entLists];
                    }

                std::ofstream write(levelPath);
                if (write.is_open())
                    {
                        write << root;
                    }
                write.close();

                globals::_logger->logToConsole("Level Saved");
            }
    }

void level::update(sf::Time deltaTime)
    {
        auto collisonComp = &collisionComponent();
        if (_player)
            {
                collisonComp = _player->get<collisionComponent>();
            }

        for (auto &ent : _entities)
            {
                auto mc = ent->get<movementComponent>();
                if (mc)
                    {
                        mc->update(deltaTime);
                        _quadtree.update(ent);
                    }
                
                if (collisonComp)
                    {
                        auto objectsInNode = _quadtree.getObjectsInRange(collisonComp->getBounds());
                        for (auto &collisionEnt : objectsInNode)
                            {
                                if (collisionEnt->getID() != _player->getID())
                                    {
                                        collisonComp->collide(collisionEnt);
                                    }

                                auto cc = collisionEnt->get<collisionComponent>();
                                if (cc && collisionEnt->getID() != _player->getID())
                                    {
                                        for (auto &otherCollisionEnt : objectsInNode)
                                            {
                                                if (otherCollisionEnt->getID() != collisionEnt->getID())
                                                    {
                                                        cc->collide(otherCollisionEnt);
                                                    }
                                            }
                                    }
                            }
                    }
            }
    }

void level::draw(sf::RenderWindow &app)
    {
		sf::FloatRect camBound(
			static_cast<sf::Vector2f>(app.getView().getCenter()) - sf::Vector2f(app.getView().getSize() / 1.5f),
			app.getView().getSize() * 1.5f);

		auto objectsInNode = _quadtree.getObjectsInRange(camBound);
        for (auto &ent : objectsInNode)
            {
                auto textureComp = ent->get<textureComponent>();
                if (textureComp)
                    {
                        textureComp->draw(app);
                    }
            }

    }

gameObject* level::addEntity(const std::string &name)
    {
        auto ent = _factory.addGameObject(name);
        if (ent)
            {
                _entities.push_back(ent);
                _quadtree.add(ent);
            }

        if (name == "player")
            {
                _player = ent;
            }

        return ent;
    }

gameObject *level::getEntityAtPosition(sf::Vector2f pos)
    {
        auto allEnts = _factory.getGameObjects();
        for (auto &ent : *allEnts)
            {
                for (auto &obj : ent.second)
                    {
                        auto tc = obj->get<textureComponent>();
                        if (tc)
                            {
                                sf::Vector2f entPos = tc->getSprite()->getPosition();
                                auto cc = obj->get<collisionComponent>();
                                if (cc)
                                    {
                                        if (clsn::hasCollided(pos, sf::Vector2f(1, 1), entPos, sf::Vector2f(cc->getBounds().width, cc->getBounds().height)))
                                            {
                                                return obj;
                                            }
                                    }
                            }
                    }
            }

        return nullptr;
    }

std::unordered_map<std::string, std::vector<gameObject*>> *level::getAllGameObjects()
    {
        return _factory.getGameObjects();
    }

std::unordered_map<std::string, std::vector<std::string>> *level::getInitializedObjects()
    {
        return _factory.getInitializedEntities();
    }

size_t level::getAmountOfGameObjectsOnLevel()
	{
		return _entities.size();
	}

quadtree *level::getQuadTree()
    {
        return &_quadtree;
    }

void level::removeEntity(gameObject *obj)
    {
        _quadtree.remove(obj);

        auto itEnt = std::remove_if(_entities.begin(), _entities.end(), [&obj] (gameObject *eObj) { return eObj->getID() == obj->getID(); });
        if (itEnt != _entities.end())
            {
                _entities.erase(itEnt);
                _factory.removeGameObject(obj);
            }
    }

level::~level()
    {
    }
