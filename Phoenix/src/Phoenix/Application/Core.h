#pragma once

#ifdef PHX_PLATFORM_WINDOWS
	#ifdef PHX_BUILD_DLL
		#define PHX_API __declspec(dllexport)
	#else
		#define PHX_API __declspec(dllimport)
	#endif 
#else
	#error Invalid platform!
#endif

#ifdef PHX_ENABLE_ASSERTS
	#define PHX_ASSERT(x, ...) { if(!(x)) { PHX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define PHX_CORE_ASSERT(x, ...) { if(!(x)) { PHX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PHX_ASSERT(x, ...)
	#define PHX_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define PHX_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)