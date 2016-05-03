#include "scriptManager.hpp"
#include "../game/globals.hpp"
#include "luaGameObject.hpp"

#include "LuaBridge/LuaBridge.h"
extern "C" 
	{
		#include <lua.h>
		#include <lauxlib.h>
		#include <lualib.h>
	}

#include "../utilities/logger/logger.hpp"
#include "../managers/events/eventManager.hpp"

using namespace luabridge;
scriptManager::scriptManager()
	{
        _state = luaL_newstate();
		luaL_openlibs(_state);

        initializeLuaHelpers();
	}

void scriptManager::initializeLuaHelpers()
    {
        getGlobalNamespace(_state).beginClass<luaGameObject>("luaGameObject")

                .addFunction("setImpulse", &luaGameObject::setImpulse)
                .addFunction("getImpulseX", &luaGameObject::getImpulseX)
                .addFunction("getImpulseY", &luaGameObject::getImpulseY)
				
				.addFunction("setPos", &luaGameObject::setPos)
				.addFunction("getPosX", &luaGameObject::getPosX)
				.addFunction("getPosY", &luaGameObject::getPosY)

				.addFunction("setSize", &luaGameObject::setSize)
				.addFunction("getSizeX", &luaGameObject::getSizeX)
				.addFunction("getSizeY", &luaGameObject::getSizeY)

                .addFunction("setMaxSpeed", &luaGameObject::setMaxSpeed)
                .addFunction("getMaxSpeed", &luaGameObject::getMaxSpeed)
            
                .addFunction("setAcceleration", &luaGameObject::setAcceleration)
                .addFunction("getAcceleration", &luaGameObject::getAcceleration)

				.addFunction("isType", &luaGameObject::isType)

				.addFunction("getDeltaTime", &luaGameObject::getDeltaTime)
                .addFunction("log", &luaGameObject::log)
                .addFunction("logToConsole", &luaGameObject::logToConsole)
            .endClass();
    }

void scriptManager::callLuaScript(const std::string &scriptPath)
	{
		luaL_dofile(_state, scriptPath.c_str());
		lua_pcall(_state, 0, 0, 0);
	}

luabridge::LuaRef *scriptManager::registerLuaFunction(const std::string &name, const std::string &scriptPath, const std::string &script)
	{
        globals::_logger->logToConsole("Adding Lua Function \"" + name + "\"\nwith script \"" + script + "\"\nat \"" + scriptPath + "\"");

		luaL_dofile(_state, scriptPath.c_str());
		luabridge::lua_pcall(_state, 0, 0, 0);
		luabridge::LuaRef *ref = new luabridge::LuaRef(luabridge::getGlobal(_state, script.c_str()));
		_luaFuncs[name] = ref;

		return _luaFuncs[name];
	}

void scriptManager::removeCFunc(const std::string &name)
	{
		auto it = _luaCFuncs.find(name);
		if (it != _luaCFuncs.end())
			{
				_luaCFuncs.erase(it);
			}
	}

void scriptManager::removeLuaFunc(const std::string &name)
	{
		auto it = _luaFuncs.find(name);
		if (it != _luaFuncs.end())
			{
				_luaFuncs.erase(it);
			}
	}

luabridge::lua_State *scriptManager::getState()
    {
        return _state;
    }

scriptManager::~scriptManager()
    {
        for (auto &ref : _luaFuncs)
            {
                delete ref.second;
                ref.second = nullptr;
            }
    }
