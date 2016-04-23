#include "scoreManager.hpp"

void scoreManager::addNewScore(const std::string &name, sf::Vector2f pos)
    {
        score newScore;
        newScore.getText().setPosition(pos);

        _scores[name] = newScore;
    }

void scoreManager::incrementScore(const std::string &name)
    {
        _scores[name].increment();
    }

void scoreManager::resetScore(const std::string & name)
    {
        _scores[name].setScore(0);
    }

score *scoreManager::getScore(const std::string &name)
    {
        auto found = _scores.find(name);
        if (found != _scores.end())
            {
                return &found->second;
            }

        return nullptr;
    }

void scoreManager::render(sf::RenderWindow &app)
    {
        for (auto &score : _scores)
            {
                score.second.render(app);
            }
    }
