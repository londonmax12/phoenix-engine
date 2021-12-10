#pragma once

#include "LuaScript.h"

#include <vector>

namespace phx {
	class LuaInstance {
	public:
		LuaInstance();
		~LuaInstance();

		Ref<LuaScript> Create();
	private:
		std::vector<Ref<LuaScript>> m_Scripts;
		lua_State* m_LuaState = nullptr;
	};
}
