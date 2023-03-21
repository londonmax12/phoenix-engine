#pragma once

#include "glm/glm.hpp"

#include <Phoenix/Application/UUID.h>
namespace phx {
	class ScriptRegistry {
	public:
		static void RegisterComponents();
		static void RegisterFunctions();
	};
}