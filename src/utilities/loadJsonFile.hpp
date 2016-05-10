// loadJsonFile.hpp
// Loads a json file into the JsonRef
#pragma once

#include <string>
#include <json/json.h>

namespace ljf
    {
        void loadJsonFile(const std::string &file, Json::Value *root);
    }