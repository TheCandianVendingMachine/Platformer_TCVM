// scriptManager.hpp
// allows the ability to register, and call scripts via string
#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

#include "LuaBridge/LuaBridge.h"
#include "../managers/events/observer.hpp"

class scriptManager
	{
		private:
			std::unordered_map<std::string, luabridge::LuaRef*> _luaFuncs;
			std::unordered_map<std::string, const void*> _luaCFuncs;
            
			luabridge::lua_State *_state;

		public:
			scriptManager();

            void initializeLuaHelpers();

			void callLuaScript(const std::string &scriptPath);
			luabridge::LuaRef *registerLuaFunction(const std::string &name, const std::string &scriptPath, const std::string &script);

            #pragma region Run Native Lua
            void runLua(const std::string &lua);
            template<typename Targ>
            void runLua(const std::string &lua, Targ &arg);
            template<typename Targ, typename Targ2>
            void runLua(const std::string &lua, Targ &arg, Targ2 &arg2);
            #pragma endregion
            #pragma region Register CPP Functions to LUA
			void registerFunctionToLua(const std::string &name, const std::string &_namespace, void (*func)(void));
            template<typename Targ>
            void registerFunctionToLua(const std::string &name, const std::string &_namespace, void(*func)(Targ));

            void registerFunctionToLua(const std::string &name, void(*func)(void));
            template<typename Targ>
			void registerFunctionToLua(const std::string &name, void (*func)(Targ));
            template<typename Targ, typename Targ2>
            void registerFunctionToLua(const std::string &name, void(*func)(Targ, Targ2));

            template<typename Targ, typename Tret>
            void registerFunctionToLua(const std::string &name, const std::string &_namespace, Tret(*func)(Targ));
            template<typename Targ, typename Tret>
            void registerFunctionToLua(const std::string &name, Tret(*func)(Targ));
            template<typename Targ, typename Targ2, typename Tret>
            void registerFunctionToLua(const std::string &name, Tret(*func)(Targ, Targ2));

            // End Register CPP Functions to LUA
            #pragma endregion
            #pragma region Calling Lua functions
			void callLuaFunc(const std::string &name);
            template<typename Targ>
            void callLuaFunc(const std::string &name, Targ &arg);
            template<typename Targ, typename Targ2>
            void callLuaFunc(const std::string &name, Targ &arg, Targ2 &arg2);
			template<typename Targ, typename Targ2, typename Targ3>
			void callLuaFunc(const std::string &name, Targ &arg, Targ2 &arg2, Targ3 &arg3);
			template<typename Targ, typename Targ2, typename Targ3, typename Targ4>
			void callLuaFunc(const std::string &name, Targ &arg, Targ2 &arg2, Targ3 &arg3, Targ4 &arg4);
            #pragma endregion

			void removeCFunc(const std::string &name);
			void removeLuaFunc(const std::string &name);

			void alert(eventData data);

            luabridge::lua_State *getState();

            ~scriptManager();

	};

#include "scriptManager.inl"