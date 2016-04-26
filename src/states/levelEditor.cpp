#include "levelEditor.hpp"

#include "../gameObject/gameObject/gameObject.hpp"
#include "../gameObject/components/textureComponent.hpp"

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

        
        globals::_mouseManager->changeFunction("editor_left_mouse_press", [this] () 
            { 
                if (!_enteringString)
                    {
                        _selectedEntity = _level->getEntityAtPosition(_mousePos);
                        if (!_selectedEntity)
                            {
                                _selectedEntity = _level->addEntity("platform");
                            }
                    }
            });
        globals::_mouseManager->changeInverseFunction("editor_left_mouse_press", [this] () 
            {
                if (!_enteringString)
                    {
                        _selectedEntity = nullptr;
                    }
            });

        globals::_keyboardManager->changeFunction("editor_delete_entity", [this] () 
            {
                if (!_enteringString)
                    {
                        _selectedEntity = _level->getEntityAtPosition(_mousePos);
                        if (_selectedEntity)
                            {
                                _level->removeEntity(_selectedEntity);
                            }

                        _selectedEntity = nullptr;
                    }
            });

        globals::_mouseManager->changeFunction("editor_right_mouse_press", [this] () 
            {
                if (!_enteringString)
                    {
                        _selectedEntity = _level->getEntityAtPosition(_mousePos);
                        if (_selectedEntity)
                            {
                                _level->removeEntity(_selectedEntity);
                            }

                        _selectedEntity = nullptr;
                    }
            });

        globals::_keyboardManager->changeFunction("editor_spin_block_right", [this] () 
            {
                if (_selectedEntity && !_enteringString)
                    {
                        auto tc = _selectedEntity->get<textureComponent>();
                        if (tc)
                            {
                                tc->getSprite()->rotate(15);
                            }
                    }
            });
        globals::_keyboardManager->changeFunction("editor_spin_block_left", [this] () 
            {
                if (_selectedEntity && !_enteringString)
                    {
                        auto tc = _selectedEntity->get<textureComponent>();
                        if (tc)
                            {
                                tc->getSprite()->rotate(-15);
                            }
                    }
            });

        globals::_keyboardManager->changeFunction("editor_save_level", [this] () 
            {
                if (!_enteringString)
                    {
                        globals::_textEntered->clearString();
                        _UIFactory.getText("loadSaveText")->setString("Save Level:");
                        _enteringString = true;
                        globals::_keyboardManager->changeFunction("editor_confirm", [this] () 
                            {
                                _level->save("assets/levels/" + globals::_textEntered->getTextEntered() + ".json");
                                globals::_keyboardManager->changeFunction("editor_confirm", [] () {});
                                _UIFactory.getText("loadSaveText")->setString("");
                                _UIFactory.getText("inputText")->setString("");
                                _enteringString = false;
                            });
                    }
                else
                    {
                        globals::_keyboardManager->changeFunction("editor_confirm", [] () {});
                        _UIFactory.getText("loadSaveText")->setString("");
                        _UIFactory.getText("inputText")->setString("");
                        _enteringString = false;
                    }
            });
        globals::_keyboardManager->changeFunction("editor_load_level", [this] () 
            {
                if (!_enteringString)
                    {
                        globals::_textEntered->clearString();
                        _UIFactory.getText("loadSaveText")->setString("Load Level:");
                        _enteringString = true;
                        globals::_keyboardManager->changeFunction("editor_confirm", [this] ()
                            {
                                _level->load("assets/levels/" + globals::_textEntered->getTextEntered() + ".json");
                                globals::_keyboardManager->changeFunction("editor_confirm", [] () {});
                                _UIFactory.getText("loadSaveText")->setString("");
                                _UIFactory.getText("inputText")->setString("");
                                _enteringString = false;
                            });
                    }
                else
                    {
                        globals::_keyboardManager->changeFunction("editor_confirm", [] () {});
                        _UIFactory.getText("loadSaveText")->setString("");
                        _UIFactory.getText("inputText")->setString("");
                        _enteringString = false;
                    }
            });

        globals::_keyboardManager->changeFunction("editor_snap_to_grid", [this] () 
                                                                                { _snapToGrid = true; });
        globals::_keyboardManager->changeInverseFunction("editor_snap_to_grid", [this] () 
                                                                                { _snapToGrid = false; });

        globals::_keyboardManager->changeFunction("editor_move_view_right", [this] () 
                                                                                { if (!_enteringString) _viewImpulse.x = 250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_right", [this] () 
                                                                                { _viewImpulse.x = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_left", [this] () 
                                                                                { if (!_enteringString) _viewImpulse.x = -250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_left", [this] () 
                                                                                { _viewImpulse.x = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_up", [this] () 
                                                                                { if (!_enteringString) _viewImpulse.y = -250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_up", [this] () 
                                                                                 { _viewImpulse.y = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_down", [this] () 
                                                                                { if (!_enteringString) _viewImpulse.y = 250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_down", [this] () 
                                                                                { _viewImpulse.y = 0; });

        globals::_keyboardManager->changeFunction("editor_zoom_view_out", [this] () 
                                                                                { if (!_selectedEntity && !_enteringString)_editorView.zoom(1.1); });
        globals::_keyboardManager->changeFunction("editor_zoom_view_in", [this] () 
                                                                                { if (!_selectedEntity&& !_enteringString) _editorView.zoom(0.9); });

    }

void levelEditor::initialize()
    {
        globals::_logger->logToConsole("Initializing level editor");

        _editorView = globals::_stateMachine->getWindow()->getView();

        _snapToGrid = false;

        _UIFactory.setFont("assets/textures/fonts/Squares_Bold_Free.otf");
        _UIFactory.add("loadSaveText", "");
        _UIFactory.add("inputText", "");
    }

void levelEditor::update(sf::Time deltaTime)
    {
        _editorView.move(_viewImpulse * deltaTime.asSeconds());

        _mousePos = _mousePosToWorldCoord();
        if (_selectedEntity)
            {
                if (_snapToGrid)
                    {
                        auto tc = _selectedEntity->get<textureComponent>();
                        if (tc)
                            {
                                tc->setPosition(_getClosestGridCoord(_mousePos));
                            }
                    }
                else
                    {
                        auto tc = _selectedEntity->get<textureComponent>();
                        if (tc)
                            {
                                tc->setPosition(_mousePos);
                            }
                    }
            }

        if (_enteringString)
            {
                _UIFactory.getText("inputText")->setString(globals::_textEntered->getTextEntered());
                
                sf::Vector2f newPos = globals::_stateMachine->getWindow()->mapPixelToCoords(sf::Vector2i(0, 0));
                _UIFactory.getText("loadSaveText")->setPosition(newPos);

                auto loadSaveTextBound = _UIFactory.getText("loadSaveText")->getGlobalBounds();
                _UIFactory.getText("inputText")->setPosition(newPos.x + loadSaveTextBound.width + 5, newPos.y);
            }

        _UIFactory.update();
    }

void levelEditor::render()
    {
        globals::_stateMachine->getWindow()->setView(_editorView);
        _UIFactory.draw(*globals::_stateMachine->getWindow());
    }

void levelEditor::cleanup()
    {
        globals::_stateMachine->getWindow()->setView(globals::_stateMachine->getWindow()->getDefaultView());
        globals::_logger->logToConsole("Cleaning up level editor");
    }

levelEditor::~levelEditor()
    {
        
    }
    