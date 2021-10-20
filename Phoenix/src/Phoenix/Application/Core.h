#pragma once

#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define PHX_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported"
	#endif	
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

namespace phx {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}