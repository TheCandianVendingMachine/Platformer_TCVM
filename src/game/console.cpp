#include "console.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

#include "../utilities/strFuncs.hpp"

#include "globals.hpp"
#include "../managers/scriptManager/scriptManager.hpp"
#include "../states/gameStates/stateMachine.hpp"
#include "../managers/events/eventManager.hpp"
#include "../utilities/strFuncs.hpp"

bool console::_handleCommand(std::string command, std::string data)
    {
        if (command == "lua")
            {
                globals::_scriptManager->runLua(data);
            }
        else if (command == "reload_entity_list")
            {
                globals::_eventManager->alert(eventData(RELOAD_ENTITY_LIST));
            }
        else if (command == "load_entity_list")
            {
                globals::_eventManager->alert(eventData(data.c_str(), LOAD_ENTITY_LIST));
            }
        else if (command == "get_fps")
            {
                log(std::to_string(1 / globals::_stateMachine->getDeltaTime()));
            }
        else if (command == "clear")
            {
                _log.clear();
            }
        else if (command == "help")
            {
                log("All Possible Commands:");
                for (auto &comand : _commands)
                    {
                        log(comand);
                    }
            }
        else
            {
                log("Error: Command \"" + command + "\" is not real");
                return false;
            }

        return true;
    }

console::console()
    {
        _commands.push_back("lua");
        _commands.push_back("reload_entity_list");
        _commands.push_back("load_entity_list");
        _commands.push_back("get_fps");
        _commands.push_back("clear");
        _commands.push_back("help");

        _active = false;
        _logged = false;
    }

void console::draw()
    {
        if (_active)
            {
                if (ImGui::Begin("Command Line", 0, ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
                    {
                        ImGui::SetWindowSize("Command Line", sf::Vector2f(globals::_stateMachine->getWindow()->getSize().x / 2, 256));
                        ImGui::BeginChild("Log", sf::Vector2f(0, ImGui::GetWindowHeight() - 64), true);
                        for (auto &entry : _log)
                            {
                                ImGui::TextWrapped(entry.c_str());
                                if (_logged)
                                    {
                                        ImGui::SetScrollPosHere();
                                    }
                            }
                        ImGui::EndChild();
                        _logged = false;

                        if (ImGui::InputText("Command", _inputBuffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                            {
                                std::string command = strfn::splitString(std::string(_inputBuffer), ' ').first;
                                std::string data = strfn::splitString(std::string(_inputBuffer), ' ').second;

                                _handleCommand(command, data);

                                strcpy_s(_inputBuffer, "");
                            }

                        ImGui::End();
                    }
            }
    }

void console::log(const std::string &message)
    {
        _log.push_back(message);
        if (_log.size() > 128)
            {
                _log.erase(_log.begin());
            }
        _logged = true;
    }

void console::setActive(bool state)
    {
        _active = state;
    }

const bool console::getActive() const
    {
        return _active;
    }
