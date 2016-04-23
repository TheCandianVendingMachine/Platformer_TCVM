#include "highscoreManager.hpp"
#include <fstream>
#include <algorithm>
#include <functional>

highscoreManager::highscoreManager() :_highscoreFilepath("assets/highscore/highscore.txt")
    {
        std::ifstream read(_highscoreFilepath);
        if (read.is_open())
            {
                std::string line = "";
                while (std::getline(read, line))
                    {
                        _highscores.push_back(std::stoi(line));
                    }
            }
    }

void highscoreManager::addScore(int score)
    {
        _highscores.push_back(score);
        std::sort(_highscores.begin(), _highscores.end(), std::greater<int>());

        std::ofstream write(_highscoreFilepath);
        if (write.is_open())
            {
                for (auto &point : _highscores)
                    {
                        write << point << std::endl;
                    }
            }
    }

std::vector<int> *highscoreManager::getScores()
    {
        return &_highscores;
    }
