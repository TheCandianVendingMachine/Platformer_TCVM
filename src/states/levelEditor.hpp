// levelEditor.hpp
// the level editor state. Takes in a level, and allows the editing of said level
#pragma once

#include "state.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

#include "../UI/userInterfaceFactory.hpp"

class level;
class gameObject;

class levelEditor : public state
    {
        private:
            userInterfaceFactory _UIFactory;

            level *_level;
            gameObject *_selectedEntity;

            sf::Vector2f _mousePos;

			bool _resizing;

            const int _gridSize;
            bool _snapToGrid;

            sf::View _editorView;
            sf::Vector2f _viewImpulse;

            bool _enteringString;

        private:
            sf::Vector2f _mousePosToWorldCoord();
            sf::Vector2i _getClosestGridCoord(sf::Vector2f pos);

        public:
            levelEditor(level *lvl);

            void initialize();

            void update(sf::Time deltaTime);
            void render();

            void cleanup();

            ~levelEditor();

    };