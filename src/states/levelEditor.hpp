// levelEditor.hpp
// the level editor state. Takes in a level, and allows the editing of said level
#pragma once

#include "state.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

#include <vector>
#include <unordered_map>

#include "../managers/events/observer.hpp"

class level;
class gameObject;

class levelEditor : public state, public observer
    {
        private:
            std::unordered_map<std::string, std::vector<std::string>> *_allGameObjects;

            level *_level;

            gameObject *_selectedEntity;
			gameObject *_holdingEntity;

            std::string _placingEntity;

            sf::Vector2f _mousePos;

            const int _gridSize;
            bool _snapToGrid;

            sf::View _editorView;
            sf::Vector2f _viewImpulse;

            bool _inUI;

            bool _save;
            bool _load;
            bool _resizing;
            bool _placeMultiple;

        private:
            sf::Vector2f _mousePosToWorldCoord();
            sf::Vector2i _getClosestGridCoord(sf::Vector2f pos);

            void handleUI();

        public:
            levelEditor(level *lvl);

            void initialize();

            void update(sf::Time deltaTime);
            void render();

            void alert(eventData data);

            void cleanup();

            ~levelEditor();

    };