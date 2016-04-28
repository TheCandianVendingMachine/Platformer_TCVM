#include "inputComponent.hpp"
#include "../../game/globals.hpp"
#include "../../managers/input/inputManager.hpp"

#include "../../utilities/json/json/json.h"

#include <logger.hpp>
#include <fstream>

void inputComponent::loadJsonFile(const std::string &file, Json::Value *root)
    {
        std::ifstream read(file);
        try
            {
                read >> *root;
            }
        catch (std::exception &e)
            {
                globals::_logger->log(file + e.what());
            }
        read.close();
    }

void inputComponent::setFuncCallStart(const std::string &call)
    {
        _funcCallStart = call;
    }

std::string inputComponent::getFuncCallStart()
    {
        return _funcCallStart;
    }

void inputComponent::setFuncCallEnd(const std::string &call)
    {
        _funcCallEnd = call;
    }

std::string inputComponent::getFuncCallEnd()
    {
        return _funcCallEnd;
    }

sf::Keyboard::Key inputComponent::setKey(const std::string &key)
    {
        Json::Value config;
        loadJsonFile("assets/config/controls.json", &config);

        auto keyNum = config.get(key, config).asInt();

        return sf::Keyboard::Key(keyNum);

    }