#pragma once
#pragma once

#include "Phoenix/Application/Base.h"
#include "Phoenix/Logging/Log.h"

#ifdef PHX_ENABLE_ASSERTS
namespace phx::Assert
{
	constexpr const char* CurrentFileName(const char* path) {
		const char* file = path;
		while (*path)
		{
			if (*path == '/' || *path == '\\')
				file = ++path;
			else
				path++;
		}
		return file;
	}
}

#define PHX_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { PHX##type##ERROR(msg, __VA_ARGS__); PHX_DEBUGBREAK(); } }
#define PHX_INTERNAL_ASSERT_WITH_MSG(type, check, ...) PHX_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define PHX_INTERNAL_ASSERT_NO_MSG(type, check) PHX_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", PHX_STRINGIFY_MACRO(check), ::phx::Assert::CurrentFileName(__FILE__), __LINE__)

#define PHX_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define PHX_INTERNAL_ASSERT_GET_MACRO(...) PHX_EXPAND_MACRO( PHX_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, PHX_INTERNAL_ASSERT_WITH_MSG, PHX_INTERNAL_ASSERT_NO_MSG) )

#define PHX_ASSERT(...) PHX_EXPAND_MACRO( PHX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define PHX_CORE_ASSERT(...) PHX_EXPAND_MACRO( PHX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define PHX_ASSERT(...)
#define PHX_CORE_ASSERT(...)
#endif