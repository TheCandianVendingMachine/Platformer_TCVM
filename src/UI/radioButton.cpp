#include "radioButton.hpp"
#include <algorithm>

void radioButton::addLinkedButton(radioButton *button)
    {
        _otherButtons.push_back(button);
    }

void radioButton::update()
    {
        _checkForMouse();
        if (_buttonClicked && !_clicked)
            {
                

                bool otherToggled = false;
                // if any other linked radio button is toggled
                // cycle through all of them and turn it off
                for (auto &otherButton : _otherButtons)
                    {
                        if (otherButton->getToggled() && !_toggle)
                            {
                                otherButton->setToggled(false);
                                otherToggled = true;
                                break;
                            }
                    }

                if (otherToggled)
                    {
                        _toggle = !_toggle;
                    }

                _clicked = true;
                setButtonTexture();
            }
        else if (!_buttonClicked && _clicked)
            {
                _clicked = false;
            }
    }
