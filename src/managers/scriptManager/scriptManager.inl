#include "scriptManager.hpp"
#include "../game/globals.hpp"
#include "../utilities/logger/logger.hpp"

inline void scriptManager::registerFunctionToLua(const std::string &name, const std::string &_namespace, void(*func)(void))
	{
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\" with namespace \"" + _namespace + "\"");
		luabridge::getGlobalNamespace(_state)
								.beginNamespace(_namespace.c_str())
									.addFunction(name.c_str(), func)
								.endNamespace();

		_luaCFuncs[name] = func;
	}

inline void scriptManager::runLua(const std::string &lua)
    {
        luaL_dostring(_state, lua.c_str());
    }

template<typename Targ>
void scriptManager::runLua(const std::string &lua, Targ &arg)
    {
        luaL_dostring(_state, lua.c_str(), arg);
    }

template<typename Targ, typename Targ2>
void scriptManager::runLua(const std::string &lua, Targ &arg, Targ2 &arg2)
    {
        luaL_dostring(_state, lua.c_str(), arg, arg2);
    }

template<typename Targ>
void scriptManager::registerFunctionToLua(const std::string &name, const std::string &_namespace, void(*func)(Targ))
	{
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\" with namespace \"" + _namespace + "\"");
		getGlobalNamespace(state)
								.beginNamespace(_namespace.c_str())
									.addFunction(name.c_str(), func)
								.endNamespace();

		_luaCFuncs[name] = func;
	}

inline void scriptManager::registerFunctionToLua(const std::string &name, void(*func)(void))
    {
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\"");
        luabridge::getGlobalNamespace(_state).addFunction(name.c_str(), func);
		_luaCFuncs[name] = func;
    }

template<typename Targ>
void scriptManager::registerFunctionToLua(const std::string &name, void(*func)(Targ))
	{
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\"");
		getGlobalNamespace(_state).addFunction(name.c_str(), func);
		_luaCFuncs[name] = func;
	}

template<typename Targ, typename Targ2>
void scriptManager::registerFunctionToLua(const std::string &name, void(*func)(Targ, Targ2))
	{
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\"");
		getGlobalNamespace(_state).addFunction(name.c_str(), func);
		_luaCFuncs[name] = func;
	}

inline void scriptManager::callLuaFunc(const std::string &name)
	{
		try 
			{
				auto func = _luaFuncs[name];
				(*func)();
			}
		catch (luabridge::LuaException &e)
			{
				globals::_logger->log(e.what());
			}
	}

template<typename Targ>
void scriptManager::callLuaFunc(const std::string &name, Targ &arg)
    {
		try 
			{
				auto func = _luaFuncs[name];
				(*func)(arg);
			}
		catch (luabridge::LuaException &e)
			{
				globals::_logger->log(e.what());
			}
    }

template<typename Targ, typename Targ2>
void scriptManager::callLuaFunc(const std::string &name, Targ &arg, Targ2 &arg2)
    {
		try 
			{
				auto func = _luaFuncs[name];
				(*func)(arg, arg2);
			}
		catch (luabridge::LuaException &e)
			{
				globals::_logger->log(e.what());
			}
    }

template<typename Targ, typename Targ2, typename Targ3>
void scriptManager::callLuaFunc(const std::string &name, Targ &arg, Targ2 &arg2, Targ3 &arg3)
	{
		try 
			{
				auto func = _luaFuncs[name];
				(*func)(arg, arg2, arg3);
			}
		catch (luabridge::LuaException &e)
			{
				globals::_logger->log(e.what());
			}
	}
