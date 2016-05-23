#include "levelEditor.hpp"

#include "../gameObject/gameObject/gameObject.hpp"
#include "../gameObject/components/textureComponent.hpp"

#include "../game/globals.hpp"
#include "../managers/scriptManager/scriptManager.hpp"
#include "level.hpp"
#include "quadtree.hpp"

#include "../utilities/logger/logger.hpp"
#include "../states/gameStates/stateMachine.hpp"
#include "../managers/input/inputManager.hpp"
#include "../managers/events/eventManager.hpp"

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
                        ImGui::Checkbox("Draw Quadtree", &_drawQuadTree);

                        ImGui::Checkbox("Snap to grid", &_snapToGrid);
                        ImGui::Checkbox("Resize", &_resizing);
                        ImGui::Checkbox("Place Multiple", &_placeMultiple);

						ImGui::Text("X:"); ImGui::SameLine(32); ImGui::Text(std::to_string(_mousePos.x).c_str());
						ImGui::Text("Y:"); ImGui::SameLine(32); ImGui::Text(std::to_string(_mousePos.y).c_str());

						ImGui::Text("Total Objects:"); ImGui::SameLine(140); ImGui::Text(std::to_string(_level->getAmountOfGameObjectsOnLevel()).c_str());

                        unsigned int amountOfObjects = 0;
                        if (_hoveringNode)
                            {
                                amountOfObjects = _hoveringNode->getObjects().size();
                                _hoveringNode->getOutline()->setOutlineColor(sf::Color::White);
                            }
						ImGui::Text("Objects In Node:"); ImGui::SameLine(140); ImGui::Text(std::to_string(amountOfObjects).c_str());
                    }
                ImGui::End();

				if (ImGui::Begin("Data"))
					{
						if (_selectedEntity)
							{
                                ImGui::Text("Entity ID:"); ImGui::SameLine(32); ImGui::Text(std::to_string(_selectedEntity->getID()).c_str());
								ImGui::Text("Components");
								for (auto &comp : *_selectedEntity->getAllComponents())
									{
										ImGui::BulletText(comp.first.name());
									}
							}
					}
				ImGui::End();

                if (ImGui::Begin("Entity Select"))
                    {
                        ImGui::BeginChild("EntitySelect", sf::Vector2f(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 64));
                        if (ImGui::TreeNode("Entities"))
                            {
                                static unsigned int selected = -1;
                                for (auto &filepath : *_allGameObjects)
                                    {
                                        for (unsigned int i = 0; i < filepath.second.size(); i++)
                                            {
                                                if (ImGui::Selectable(filepath.second[i].c_str(), selected == i)) 
                                                    { 
                                                        if (selected != i)
                                                            {
                                                                _placingEntity = filepath.second[i];
                                                                selected = i;
                                                            }
                                                        else
                                                            {
                                                                _placingEntity = "";
                                                                selected = -1;
                                                            }
                                                    }
                                            }
                                    }
                                ImGui::TreePop();
                            }
                        ImGui::EndChild();
                    }

                if (_save || _load)
                    {
                        if (ImGui::Begin(_save ? "Save" : "Load"))
                            {
                                static char _inputBuffer[128];
                                if (ImGui::InputText("File Name", _inputBuffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                                    {
                                        if (_save)
                                            {
                                                _level->save("assets/levels/" + std::string(_inputBuffer));
                                            }
                                        else
                                            {
                                                _level->load("assets/levels/" + std::string(_inputBuffer));
                                            }
                                        strcpy_s(_inputBuffer, "");
                                    }
                                ImGui::End();
                            }
                    }
                ImGui::End();
                
                _inUI = ImGui::IsMouseHoveringAnyWindow();
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

        _entityBoundingBox.setFillColor(sf::Color::Transparent);
        _entityBoundingBox.setOutlineThickness(5);
        _entityBoundingBox.setOutlineColor(sf::Color::Red);

    #pragma region Setting up editor key binds
        globals::_mouseManager->changeFunction("editor_left_mouse_press", [this] () 
            { 
                if (!_inUI)
                    {
						_holdingEntity = _selectedEntity;
                        if (!_holdingEntity && !_placingEntity.empty())
                            {
                                _holdingEntity = _level->addEntity(_placingEntity);
                            }
                    }
            });
        globals::_mouseManager->changeInverseFunction("editor_left_mouse_press", [this] () 
            {
                if (!_inUI)
                    {
						_holdingEntity = nullptr;
                    }
            });

        globals::_keyboardManager->changeFunction("editor_delete_entity", [this] () 
            {
                if (!_inUI)
                    {
                        if (_selectedEntity)
                            {
                                _level->removeEntity(_selectedEntity);
                            }
                    }
            });

        globals::_keyboardManager->changeFunction("editor_place_multiple", [this] () 
            {
                if (!_inUI)
                    {
                        _placeMultiple = false;
                    }
            });

        globals::_keyboardManager->changeInverseFunction("editor_place_multiple", [this] () 
            {
                if (!_inUI)
                    {
                        _placeMultiple = true;
                    }
            });

        globals::_mouseManager->changeFunction("editor_right_mouse_press", [this] () 
            {
                if (!_inUI)
                    {
                        if (_selectedEntity)
                            {
                                _level->removeEntity(_selectedEntity);
                            }
                    }
            });

        globals::_keyboardManager->changeFunction("editor_spin_block_right", [this] () 
            {
                if (!_inUI)
                    {
                        if (_selectedEntity)
                            {
                                auto tc = _selectedEntity->get<textureComponent>();
                                if (tc)
                                    {
                                        tc->setRotation(tc->getRotation() + 90);
                                    }
                            }
                    }
            });
        globals::_keyboardManager->changeFunction("editor_spin_block_left", [this] () 
            {
                if (!_inUI)
                    {
                        if (_selectedEntity)
                            {
                                auto tc = _selectedEntity->get<textureComponent>();
                                if (tc)
                                    {
                                        tc->setRotation(tc->getRotation() - 90);
                                    }
                            }
                    }
            });

        globals::_keyboardManager->changeFunction("editor_save_level", [this] ()
            { 
                _save = !_save; 
                if (_load) 
                    { 
                        _load = false;
                    }
            });
        globals::_keyboardManager->changeFunction("editor_load_level", [this] ()
            { 
                _load = !_load;
                if (_save) 
                    { 
                        _save = false;
                    }
            });

        globals::_keyboardManager->changeFunction("editor_move_view_right", [this] () 
            { 
                if (!_inUI) 
                    { 
                        _viewImpulse.x = 250;
                    }
            });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_right", [this] () 
            { 
                _viewImpulse.x = 0;
            });

        globals::_keyboardManager->changeFunction("editor_move_view_left", [this] () 
            {
                if (!_inUI) 
                    { 
                        _viewImpulse.x = -250;
                    }
            });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_left", [this] () 
            { 
                _viewImpulse.x = 0;
            });

        globals::_keyboardManager->changeFunction("editor_move_view_up", [this] () 
            {
                if (!_inUI) 
                    { 
                        _viewImpulse.y = -250;
                    }
            });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_up", [this] () 
            { 
                _viewImpulse.y = 0;
            });

        globals::_keyboardManager->changeFunction("editor_move_view_down", [this] () 
            { 
                if (!_inUI)
                    { 
                        _viewImpulse.y = 250;
                    }
            });
        globals::_keyboardManager->changeInverseFunction("editor_move_view_down", [this] () 
            { 
                _viewImpulse.y = 0;
            });

        globals::_keyboardManager->changeFunction("editor_zoom_view_out", [this] () 
            { 
                if (!_inUI) 
                    {
                        if (!_selectedEntity)
                            {
                                _editorView.zoom(1.1f);
                            }
                    }
            });
        globals::_keyboardManager->changeFunction("editor_zoom_view_in", [this] () 
            { 
                if (!_inUI) 
                    {
                        if (!_selectedEntity) 
                            {
                                _editorView.zoom(0.9f);
                            }
                    }
            });
    #pragma endregion
    }

void levelEditor::initialize()
    {
        globals::_logger->logToConsole("Initializing level editor");

        _snapToGrid = false;
        _resizing = false;
        _placeMultiple = true;

        _load = false;
        _save = false;

        _inUI = false;

        _selectedEntity = nullptr;

        _placingEntity = "";

        globals::_eventManager->subscribe(this, events::LOAD_ENTITY_LIST);
        globals::_eventManager->subscribe(this, events::RELOAD_ENTITY_LIST);

        _previousView = globals::_stateMachine->getWindow()->getView();
        _editorView = globals::_stateMachine->getWindow()->getDefaultView();
		_editorView.setCenter(_previousView.getCenter());
    }

void levelEditor::update(sf::Time deltaTime)
    {
        _editorView.move(_viewImpulse * deltaTime.asSeconds());

        _mousePos = _mousePosToWorldCoord();
		_selectedEntity = _level->getEntityAtPosition(_mousePos);

        if (_hoveringNode)
            {
                sf::Color normalColor(144, 40, 40, 200);
                if (_hoveringNode->getOutline()->getOutlineColor() != normalColor)
                    {
                        _hoveringNode->getOutline()->setOutlineColor(normalColor);
                    }
            }
        _hoveringNode = _level->getQuadTree()->getNode(_mousePos);

        if (_holdingEntity)
            {
                auto tc = _holdingEntity->get<textureComponent>();
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

                _level->getQuadTree()->update(_holdingEntity);
            }

        if (_selectedEntity || _holdingEntity)
            {
                gameObject *ent = nullptr;

                if (_selectedEntity)
                    {
                        ent = _selectedEntity;
                    }
                else
                    {
                        ent = _holdingEntity;
                    }

                _hoveringNode = _level->getQuadTree()->getNode(ent);
                auto tc = ent->get<textureComponent>();
                if (tc)
                    {
                        _entityBoundingBox.setSize(tc->getSize());
                        _entityBoundingBox.setRotation(tc->getRotation());
                        _entityBoundingBox.setPosition(tc->getPosition());
                    }
            }
    }

void levelEditor::render()
    {
        globals::_stateMachine->getWindow()->setView(_editorView);

        if (_holdingEntity || _selectedEntity)
            {
                globals::_stateMachine->getWindow()->draw(_entityBoundingBox);
            }

        if (_drawQuadTree)
            {
                _level->getQuadTree()->draw(*globals::_stateMachine->getWindow());
            }

        handleUI();
    }

void levelEditor::alert(eventData data)
    {
        switch (data._event)
            {
                case events::LOAD_ENTITY_LIST:
                case events::RELOAD_ENTITY_LIST:
                    _allGameObjects = _level->getInitializedObjects();
                    break;
                default:
                    break;
            }
    }

void levelEditor::cleanup()
    {
        globals::_logger->logToConsole("Cleaning up level editor");

        _inUI = false;

        globals::_eventManager->unsubscribe(this, events::LOAD_ENTITY_LIST);
        globals::_eventManager->unsubscribe(this, events::RELOAD_ENTITY_LIST);

        globals::_stateMachine->getWindow()->setView(_previousView);
    }

levelEditor::~levelEditor()
    {
        
    }
    