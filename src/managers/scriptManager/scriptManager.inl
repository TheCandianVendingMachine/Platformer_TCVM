inline void scriptManager::registerFunctionToLua(const std::string &name, const std::string &_namespace, void(*func)(void))
	{
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\" with namespace \"" + _namespace + "\"");
		getGlobalNamespace(state)
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
        getGlobalNamespace(state).addFunction(name.c_str(), func);
		_luaCFuncs[name] = func;
    }

template<typename Targ>
void scriptManager::registerFunctionToLua(const std::string &name, void(*func)(Targ))
	{
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\"");
		getGlobalNamespace(state).addFunction(name.c_str(), func);
		_luaCFuncs[name] = func;
	}

template<typename Targ, typename Targ2>
void scriptManager::registerFunctionToLua(const std::string &name, void(*func)(Targ, Targ2))
	{
        globals::_logger->logToConsole("Adding CPP Function \"" + name + "\"");
		getGlobalNamespace(state).addFunction(name.c_str(), func);
		_luaCFuncs[name] = func;
	}