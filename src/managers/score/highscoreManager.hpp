// highscoreManager.hpp
// manages highscores. Sorts them and writes/reads them to files
#pragma once

#include <vector>
#include <string>

class highscoreManager
    {
        private:
            std::vector<int> _highscores;
            const std::string _highscoreFilepath;

        public:
            highscoreManager();
            
            void addScore(int score);
            std::vector<int> *getScores();

    };