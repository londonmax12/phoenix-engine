#pragma once

#include <memory>

#include <memory>

// Platform detection using predefined macros
#ifndef PHX_PLATFORM_WINDOWS
#ifdef _WIN32
#ifdef _WIN64
#define PHX_PLATFORM_WINDOWS
#else
#error "x86 Builds are not supported!"
#endif
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define PHX_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define PHX_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
#elif defined(__ANDROID__)
#define PHX_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define PHX_PLATFORM_LINUX
#error "Linux is not supported!"
#else
#ifndef PHX_PLATFORM_WINDOWS
#error "Unknown platform!"
#endif 
#endif

#ifdef PHX_DEBUG
#define PHX_ENABLE_ASSERTS
#endif
#ifdef PHX_ENABLE_ASSERTS
	#define PHX_ASSERT(x, ...) { if(!(x)) { PHX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define PHX_CORE_ASSERT(x, ...) { if(!(x)) { PHX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PHX_ASSERT(x, ...)
	#define PHX_CORE_ASSERT(x, ...) 
#endif

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace phx {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}