#include "loadJsonFile.hpp"
#include "../game/globals.hpp"
#include "logger/logger.hpp"
#include <fstream>

void ljf::loadJsonFile(const std::string &file, Json::Value *root)
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
