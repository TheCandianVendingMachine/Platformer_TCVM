// console.hpp
// active in all states. Allows for commands to be inputted, and gets interepereted
#pragma once

#include <vector>
#include <string>

class console
    {
        private:
            char _inputBuffer[128];

            std::vector<std::string> _commands;
            std::vector<std::string> _log;

            bool _active;
            bool _logged;

            bool _handleCommand(std::string command, std::string data);

        public:
            console();

            void draw();
            void log(const std::string &message);

            void setActive(bool state);
            const bool getActive() const;

    };