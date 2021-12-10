#include "phxpch.h"
#include "LuaInstance.h"

namespace phx
{
	LuaInstance::LuaInstance()
	{
		m_LuaState = luaL_newstate();
	}
	LuaInstance::~LuaInstance()
	{
		if (m_LuaState)
			lua_close(m_LuaState);
	}
	Ref<LuaScript> LuaInstance::Create()
	{
		LuaScript newScript = LuaScript(m_LuaState);
		Ref<LuaScript> newScriptRef = CreateRef<LuaScript>(newScript);

		m_Scripts.push_back(newScriptRef);
		return newScriptRef;
	}
}

