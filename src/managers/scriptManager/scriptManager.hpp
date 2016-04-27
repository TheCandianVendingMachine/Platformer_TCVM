// scriptManager.hpp
// allows the ability to register, and call scripts via string
#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

#include "LuaBridge/LuaBridge.h"

class scriptManager
	{
		private:
			std::unordered_map<std::string, luabridge::LuaRef*> _luaFuncs;
			std::unordered_map<std::string, const void*> _luaCFuncs;

			luabridge::lua_State *state;

		public:
			scriptManager();

			void callLuaScript(const std::string &scriptPath);

			void registerLuaFunction(const std::string &name, const std::string &scriptPath, const std::string &script);

            #pragma region Register CPP Functions to LUA
			void registerFunctionToLua(const std::string &name, const std::string &_namespace, void (*func)(void));
            template<typename Targ>
            void registerFunctionToLua(const std::string &name, const std::string &_namespace, void(*func)(Targ));

            void registerFunctionToLua(const std::string &name, void(*func)(void));
            template<typename Targ>
			void registerFunctionToLua(const std::string &name, void (*func)(Targ));
            template<typename Targ, typename Targ2>
            void registerFunctionToLua(const std::string &name, void(*func)(Targ, Targ2));
            // End Register CPP Functions to LUA
            #pragma endregion

			void callLuaFunc(const std::string &name);

            ~scriptManager();

	};

#include "scriptManager.inl"