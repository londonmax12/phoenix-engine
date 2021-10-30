#pragma once

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
