#include "scriptManager.hpp"

#include "../../game/globals.hpp"
#include <logger.hpp>

#include "LuaBridge/LuaBridge.h"
extern "C" 
	{
		#include <lua.h>
		#include <lauxlib.h>
		#include <lualib.h>
	}

using namespace luabridge;
scriptManager::scriptManager()
	{
        globals::_logger->logToConsole("Initializing Script Manager");
		state = luaL_newstate();
		luaL_openlibs(state);
	}

void scriptManager::callLuaScript(const std::string &scriptPath)
	{
		luaL_dofile(state, scriptPath.c_str());
		lua_pcall(state, 0, 0, 0);
	}

void scriptManager::registerLuaFunction(const std::string &name, const std::string &scriptPath, const std::string &script)
	{
        globals::_logger->logToConsole("Adding Lua Function \"" + name + "\" with script \"" + script + "\" at \"" + scriptPath + "\"");

		luaL_dofile(state, scriptPath.c_str());
		lua_pcall(state, 0, 0, 0);
		luabridge::LuaRef *ref = new luabridge::LuaRef(luabridge::getGlobal(state, script.c_str()));
		_luaFuncs[name] = ref;
	}

void scriptManager::callLuaFunc(const std::string &name)
	{
        auto func = _luaFuncs[name];
        (*func)();
	}

scriptManager::~scriptManager()
    {
        for (auto &ref : _luaFuncs)
            {
                delete ref.second;
                ref.second = nullptr;
            }
    }
