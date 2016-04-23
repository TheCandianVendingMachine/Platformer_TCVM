#include "levelEditor.hpp"

#include "../entity/entity.hpp"
#include "../game/globals.hpp"
#include "gameplay/level.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

sf::Vector2f levelEditor::_mousePosToWorldCoord()
    {
        sf::Vector2i mousePosInt(sf::Mouse::getPosition(*globals::_stateMachine->getWindow()));
        return globals::_stateMachine->getWindow()->mapPixelToCoords(mousePosInt);
    }

sf::Vector2f levelEditor::_getClosestGridCoord(sf::Vector2f pos)
    {
        return sf::Vector2f(pos.x - (static_cast<int>(pos.x) % _gridSize),
                            pos.y - (static_cast<int>(pos.y) % _gridSize));
    }

levelEditor::levelEditor(level *lvl) : _gridSize(16)
    {
        _level = lvl;

        _renderOvertop = true;
        _updateUnderneath = false;
        _state = LEVEL_EDITOR_STATE;

        _viewImpulse = sf::Vector2f(0, 0);

        
        globals::_mouseManager->changeFunction("editor_mouse_press", [this] () 
            { 
                sf::Vector2f mousePos = _mousePosToWorldCoord();
                _selectedEntity = _level->getPlatformAt(mousePos);
                if (!_selectedEntity)
                    {
                        _selectedEntity = _level->addPlatform();
                    }
            });
        globals::_mouseManager->changeFunction("editor_mouse_release", [this] () 
            {
                _selectedEntity = nullptr;
            });

        globals::_keyboardManager->changeFunction("editor_delete_entity", [this] () 
            {
                sf::Vector2f mousePos = _mousePosToWorldCoord();
                _selectedEntity = _level->getPlatformAt(mousePos);
                if (_selectedEntity)
                    {
                        _level->removePlatform(_selectedEntity);
                    }

                _selectedEntity = nullptr;
            });

        globals::_keyboardManager->changeFunction("editor_snap_to_grid", [this] () { _snapToGrid = true; });
        globals::_keyboardManager->changeInverseFunction("editor_snap_to_grid", [this] () { _snapToGrid = false; });

        globals::_keyboardManager->changeFunction("editor_move_view_right", [this] () { _viewImpulse.x = 250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_right", [this] () { _viewImpulse.x = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_left", [this] () { _viewImpulse.x = -250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_left", [this] () { _viewImpulse.x = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_up", [this] () { _viewImpulse.y = -250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_up", [this] () { _viewImpulse.y = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_down", [this] () { _viewImpulse.y = 250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_down", [this] () { _viewImpulse.y = 0; });

    }

void levelEditor::initialize()
    {
        globals::_logger->logToConsole("Editor Activated");
        _editorView = globals::_stateMachine->getWindow()->getView();

        _snapToGrid = false;
    }

void levelEditor::update(sf::Time deltaTime)
    {
        _editorView.move(_viewImpulse * deltaTime.asSeconds());

        sf::Vector2f mousePos = _mousePosToWorldCoord();
        if (_selectedEntity)
            {
                if (_snapToGrid)
                    {
                        _selectedEntity->setPosition(_getClosestGridCoord(mousePos));
                    }
                else
                    {
                        _selectedEntity->setPosition(mousePos.x, mousePos.y);
                    }
            }

        // handle UI hover over here.
    }

void levelEditor::render()
    {
        globals::_stateMachine->getWindow()->setView(_editorView);
        // draw editor UI here
    }

void levelEditor::cleanup()
    {
        globals::_stateMachine->getWindow()->setView(globals::_stateMachine->getWindow()->getDefaultView());
        globals::_logger->logToConsole("Editor Deactivated");
    }

levelEditor::~levelEditor()
    {
        
    }
