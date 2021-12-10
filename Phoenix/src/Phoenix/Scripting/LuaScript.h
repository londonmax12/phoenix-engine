#pragma once

#include <../vendor/Lua/lua.hpp>

#include "Phoenix/Scripting/ScriptingDataTypes.h"

#include <string>

namespace phx {
	class LuaScript {
	public:
		LuaScript(lua_State* ls);
		~LuaScript();

		void SetCode(const char* code);
		void LoadFile(std::string filepath);

		int GetGlobal(std::string varName);
		int GetGlobalInt(std::string varName);
		
		lua_State* GetLuaState() { return m_LuaState; }

		void Run();

		void RunFunction(std::string funcName);

		std::string m_Name;

		template<typename Lambda>
		void PushFunction(std::string funcName, Lambda func)
		{
			lua_pushcfunction(m_LuaState, func);
			lua_setglobal(m_LuaState, funcName.c_str());
		}
		template<typename T>
		void PushVariable(std::string varName, ScriptingDataType type, T& value)
		{
			switch (type)
			{
			case phx::Null:
			{
				lua_pushnil(m_LuaState);
				break;
			}
			case phx::Number:
			{
				lua_pushnumber(m_LuaState, (lua_Number)value);
				break;
			}
			case phx::Bool:
			{
				lua_pushboolean(m_LuaState, (int)value);
				break;
			}
			case phx::String:
			{
				lua_pushstring(m_LuaState, (const char*)value);
				break;
			}
			case phx::Function:
			{
				lua_pushcfunction(m_LuaState, value);
				break;
			}
			case phx::Thread:
			{
				lua_pushthread(m_LuaState);
				break;
			}
			case phx::Table:
			{
				lua_pushglobaltable(m_LuaState, value);
				break;
			}
			case phx::LightUserData:
			{
				lua_pushlightuserdata(m_LuaState, value)
					break;
			}
			case phx::UserData:
			{
				break;
			}
			default:
				break;
			}
		}
	private:
		lua_State* m_LuaState = nullptr;

		const char* m_Code = "";
	};
}