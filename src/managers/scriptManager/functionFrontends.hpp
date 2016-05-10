// functionFrontends.hpp
// Add functions from different classes in here, and register in script component to make
// then compatable with lua
#pragma once

#include <string>

namespace frontendFuncs
    {
        void alert(const std::string &event);
    }