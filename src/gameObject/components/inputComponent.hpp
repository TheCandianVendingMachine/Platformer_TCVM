// inputComponent.hpp
// allows the character to move
#pragma once

#include "component.hpp"

#include <SFML/Window/Keyboard.hpp> 
#include <string>

namespace Json
    {
        class Value;
    }

class inputComponent : public component
    {
        private:
            std::string _funcCallStart;
            std::string _funcCallEnd;

            void loadJsonFile(const std::string &file, Json::Value *root);

        public:
            void setFuncCallStart(const std::string &call);
            std::string getFuncCallStart();

            void setFuncCallEnd(const std::string &call);
            std::string getFuncCallEnd();

            sf::Keyboard::Key setKey(const std::string &key);

    };