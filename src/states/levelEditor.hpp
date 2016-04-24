// levelEditor.hpp
// the level editor state. Takes in a level, and allows the editing of said level
#pragma once

#include "state.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

class level;
class entity;

class levelEditor : public state
    {
        private:
            level *_level;
            //entity *_selectedEntity;

            sf::Vector2f _mousePos;

            const int _gridSize;
            bool _snapToGrid;

            sf::View _editorView;
            sf::Vector2f _viewImpulse;

        private:
            sf::Vector2f _mousePosToWorldCoord();
            sf::Vector2f _getClosestGridCoord(sf::Vector2f pos);

        public:
            levelEditor(level *lvl);

            void initialize();

            void update(sf::Time deltaTime);
            void render();

            void cleanup();

            ~levelEditor();

    };