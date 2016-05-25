#include "userInterfaceFactory.hpp"
#include "button.hpp"

#include "../utilities/logger/logger.hpp"

void userInterfaceFactory::setFont(const std::string &filepath)
    {
        _fonts.add(filepath, "UIFont");
    }

sf::Font *userInterfaceFactory::getFont()
    {
        return _fonts.get("UIFont", false);
    }

void userInterfaceFactory::add(const std::string &name, const std::string &text)
    {
        sf::Text newText;
        newText.setFont(*_fonts.get("UIFont", false));
        newText.setString(text);

        _UIText[name] = newText;
    }

void userInterfaceFactory::add(const std::string &name, button *obj)
    {
        _UIButtons[name] = obj;
    }

button *userInterfaceFactory::getButton(const std::string &name)
    {
        auto it = _UIButtons.find(name);
        if (it != _UIButtons.end())
            {
                return it->second;
            }

        globals::_logger->log("Cannot get button \"" + name + "\"");
        return nullptr;
    }

sf::Text *userInterfaceFactory::getText(const std::string &name)
    {
        auto it = _UIText.find(name);
        if (it != _UIText.end())
            {
                return &it->second;
            }

        globals::_logger->log("Cannot get text \"" + name + "\"");
        return nullptr;
    }

void userInterfaceFactory::draw(sf::RenderWindow &app)
    {
        for (auto &elem : _UIButtons)
            {
                elem.second->render(app);
            }

        for (auto &elem : _UIText)
            {
                app.draw(elem.second);
            }
    }

void userInterfaceFactory::update()
    {
        for (auto &elem : _UIButtons)
            {
                elem.second->update();
            }
    }

userInterfaceFactory::~userInterfaceFactory()
    {
    }
