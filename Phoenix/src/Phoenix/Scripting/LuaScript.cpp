#include "phxpch.h"
#include "LuaScript.h"

namespace phx {
	LuaScript::LuaScript(lua_State* ls)
	{
		m_LuaState = ls;
	}
	LuaScript::~LuaScript()
	{
	}

	void LuaScript::SetCode(const char* code)
	{
		m_Code = code;
	}

	void LuaScript::LoadFile(std::string filepath)
	{
		luaL_dofile(m_LuaState, filepath.c_str());
	}

	int LuaScript::GetGlobal(std::string varName)
	{
		return lua_getglobal(m_LuaState, varName.c_str());
	}

	int LuaScript::GetGlobalInt(std::string varName)
	{
		lua_getglobal(m_LuaState, varName.c_str());
		if (lua_isinteger(m_LuaState, 1))
		{
			lua_Number num = lua_tonumber(m_LuaState, 1);
			return num;
		}
		else
			return 0;
	}
	void LuaScript::Run()
	{
		luaL_dostring(m_LuaState, m_Code);
	}

	void LuaScript::RunFunction(std::string funcName)
	{
		lua_getglobal(m_LuaState, funcName.c_str());
		if (lua_isfunction(m_LuaState, 1))
		{
			lua_pcall(m_LuaState, 0, 1, 0);
			return;
		}
		else
			return;
	}
}

