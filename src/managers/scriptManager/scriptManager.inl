#include "scriptManager.hpp"
#include "../../game/globals.hpp"
#include <logger.hpp>

inline void scriptManager::registerFunctionToLua(const std::string &name, const std::string &_namespace, void(*func)(void))
	{
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\" with namespace \"" + _namespace + "\"");
		luabridge::getGlobalNamespace(_state)
								.beginNamespace(_namespace.c_str())
									.addFunction(name.c_str(), func)
								.endNamespace();

		_luaCFuncs[name] = func;
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
        auto func = _luaFuncs[name];
        (*func)();
	}

template<typename Targ>
void scriptManager::callLuaFunc(const std::string &name, Targ &arg)
    {
        auto func = _luaFuncs[name];
        (*func)(arg);
    }

template<typename Targ, typename Targ2>
void scriptManager::callLuaFunc(const std::string &name, Targ &arg, Targ2 &arg2)
    {
        auto func = _luaFuncs[name];
        (*func)(arg, arg2);
    }
