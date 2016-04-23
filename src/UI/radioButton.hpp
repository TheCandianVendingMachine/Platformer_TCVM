// radioButton.hpp
// button that can only have 1 selected in a group
#pragma once

#include "checkBox.hpp"

class radioButton : public checkBox
    {
        private:
            std::vector<radioButton*> _otherButtons;

        public:
            void addLinkedButton(radioButton *button);

            void update();

    };