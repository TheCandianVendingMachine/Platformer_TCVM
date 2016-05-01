#include "levelEditor.hpp"

#include "../gameObject/gameObject/gameObject.hpp"
#include "../gameObject/components/textureComponent.hpp"

#include "../game/globals.hpp"
#include "../managers/scriptManager/scriptManager.hpp"
#include "gameplay/level.hpp"

#include <logger.hpp>
#include "../states/stateMachine.hpp"
#include "../managers/input/inputManager.hpp"

#include "imgui.h"
#include "imgui-sfml.h"

#include <SFML/Graphics/RenderWindow.hpp>

sf::Vector2f levelEditor::_mousePosToWorldCoord()
    {
        sf::Vector2i mousePosInt(sf::Mouse::getPosition(*globals::_stateMachine->getWindow()));
        return globals::_stateMachine->getWindow()->mapPixelToCoords(mousePosInt);
    }

sf::Vector2i levelEditor::_getClosestGridCoord(sf::Vector2f pos)
    {
        return sf::Vector2i(static_cast<int>(pos.x) - (static_cast<int>(pos.x) % _gridSize),
                            static_cast<int>(pos.y) - (static_cast<int>(pos.y) % _gridSize));
    }

void levelEditor::handleUI()
    {
        if (globals::_stateMachine->getWindow()->hasFocus())
            {
                if (ImGui::Begin("Options"))
                    {
                        ImGui::Checkbox("Snap to grid", &_snapToGrid);
                        ImGui::Checkbox("Resize", &_resizing);

                        ImGui::Text("Place Multiple: "); ImGui::SameLine(120); ImGui::Text(_placeMultiple ? "True" : "False");
                    }
                ImGui::End();

                if (ImGui::Begin("Entity Select"))
                    {
                        if (ImGui::TreeNode("Entities"))
                            {
                                for (auto &filepath : *_allGameObjects)
                                    {
                                        for (auto &entName : filepath.second)
                                            {
                                                if (ImGui::Selectable(entName.c_str())) _placingEntity = entName;
                                            }
                                    }
                                ImGui::TreePop();
                            }
                    }
                ImGui::End();
            }
    }

levelEditor::levelEditor(level *lvl) : _gridSize(16)
    {
        _level = lvl;
        _allGameObjects = _level->getInitializedObjects();

        _renderOvertop = true;
        _updateUnderneath = false;
        _state = LEVEL_EDITOR_STATE;

        _viewImpulse = sf::Vector2f(0, 0);

		_resizing = false;
        _placeMultiple = false;

        _selectedEntity = nullptr;
        _placingEntity = "";

        globals::_mouseManager->changeFunction("editor_left_mouse_press", [this] () 
            { 
                _selectedEntity = _level->getEntityAtPosition(_mousePos);
                if (!_selectedEntity && !_placingEntity.empty())
                    {
                        _selectedEntity = _level->addEntity(_placingEntity);

                        if (!_placeMultiple) _placingEntity = "";
                    }
            });
        globals::_mouseManager->changeInverseFunction("editor_left_mouse_press", [this] () 
            {
                _selectedEntity = nullptr;
            });

        globals::_keyboardManager->changeFunction("editor_delete_entity", [this] () 
            {
                _selectedEntity = _level->getEntityAtPosition(_mousePos);
                if (_selectedEntity)
                    {
                        _level->removeEntity(_selectedEntity);
                    }

                _selectedEntity = nullptr;
            });
        globals::_keyboardManager->changeFunction("editor_place_multiple", [this] () 
            {
                _placeMultiple = true;
            });
        globals::_keyboardManager->changeInverseFunction("editor_place_multiple", [this] () 
            {
                _placeMultiple = false;
                _placingEntity = "";
            });

        globals::_mouseManager->changeFunction("editor_right_mouse_press", [this] () 
            {
                _selectedEntity = _level->getEntityAtPosition(_mousePos);
                if (_selectedEntity)
                    {
                        _level->removeEntity(_selectedEntity);
                    }

                _selectedEntity = nullptr;
            });

        globals::_keyboardManager->changeFunction("editor_spin_block_right", [this] () 
            {
                if (_selectedEntity)
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
                if (_selectedEntity)
                    {
                        auto tc = _selectedEntity->get<textureComponent>();
                        if (tc)
                            {
                                tc->getSprite()->rotate(-15);
                            }
                    }
            });

        globals::_keyboardManager->changeFunction("editor_move_view_right", [this] () 
                                                                                { _viewImpulse.x = 250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_right", [this] () 
                                                                                { _viewImpulse.x = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_left", [this] () 
                                                                                { _viewImpulse.x = -250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_left", [this] () 
                                                                                { _viewImpulse.x = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_up", [this] () 
                                                                                { _viewImpulse.y = -250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_up", [this] () 
                                                                                 { _viewImpulse.y = 0; });

        globals::_keyboardManager->changeFunction("editor_move_view_down", [this] () 
                                                                                { _viewImpulse.y = 250; });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_down", [this] () 
                                                                                { _viewImpulse.y = 0; });

        globals::_keyboardManager->changeFunction("editor_zoom_view_out", [this] () 
                                                                                { if (!_selectedEntity)_editorView.zoom(1.1); });
        globals::_keyboardManager->changeFunction("editor_zoom_view_in", [this] () 
                                                                                { if (!_selectedEntity) _editorView.zoom(0.9); });

    }

void levelEditor::initialize()
    {
        globals::_logger->logToConsole("Initializing level editor");

        _editorView = globals::_stateMachine->getWindow()->getView();

        _snapToGrid = false;
    }

void levelEditor::update(sf::Time deltaTime)
    {
        _editorView.move(_viewImpulse * deltaTime.asSeconds());

        _mousePos = _mousePosToWorldCoord();
        if (_selectedEntity)
            {
                auto tc = _selectedEntity->get<textureComponent>();
				if (!_resizing)
					{
                        if (tc)
                            {
						        if (_snapToGrid)
							        {
								        tc->setPosition(sf::Vector2f(_getClosestGridCoord(_mousePos)));
							        }
						        else
							        {
								        tc->setPosition(_mousePos);
							        }
                            }
					}
				else
					{
						if (tc)
							{
								sf::Vector2f objSize = tc->getSize();
								sf::Vector2f objPos = tc->getSprite()->getPosition();

								sf::Vector2f size = _mousePos - objPos;

                                if (_snapToGrid)
                                    {
                                        tc->setSize(sf::Vector2f(_getClosestGridCoord(size)));
                                    }
                                else
                                    {
                                        tc->setSize(size);
                                    }
							}
					}
            }
    }

void levelEditor::render()
    {
        globals::_stateMachine->getWindow()->setView(_editorView);

        handleUI();
        ImGui::Render();
    }

void levelEditor::cleanup()
    {
        globals::_stateMachine->getWindow()->setView(globals::_stateMachine->getWindow()->getDefaultView());
        globals::_logger->logToConsole("Cleaning up level editor");
    }

levelEditor::~levelEditor()
    {
        
    }
    