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
