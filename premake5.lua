workspace "Phoenix"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Phoenix"
	location "Phoenix"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "phxpch.h"
	pchsource "Phoenix/src/phxpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PHX_PLATFORM_WINDOWS",
			"PHX_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PHX_DEBUG_MODE"
		symbols "On"

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		optimize "On"

	filter "configurations:Dist_MODE"
		defines "PHX_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Phoenix/vendor/spdlog/include",
		"Phoenix/src"
	}

	links
	{
		"Phoenix"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PHX_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines 
		{
			"PHX_DEBUG_MODE",
			"PHX_ENABLE_ASSERTS"
		}
		symbols "On"

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		optimize "On"

	filter "configurations:Dist"
		defines "PHX_DIST_MODE"
		optimize "On"


project "GLFW"
	location "GLFW"
	kind "StaticLib"
	language "C"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Phoenix/vendor/GLFW/include/GLFW/glfw3.h",
		"Phoenix/vendor/GLFW/include/GLFW/glfw3native.h",
		"Phoenix/vendor/GLFW/src/glfw_config.h",
		"Phoenix/vendor/GLFW/src/context.c",
		"Phoenix/vendor/GLFW/src/init.c",
		"Phoenix/vendor/GLFW/src/input.c",
		"Phoenix/vendor/GLFW/src/monitor.c",
		"Phoenix/vendor/GLFW/src/vulkan.c",
		"Phoenix/vendor/GLFW/src/window.c"
	}
	
	filter "system:windows"
		buildoptions { "-std=c11", "-lgdi32" }
		systemversion ""
		staticruntime "On"
		
		files
		{
			"Phoenix/vendor/GLFW/src/win32_init.c",
			"Phoenix/vendor/GLFW/src/win32_joystick.c",
			"Phoenix/vendor/GLFW/src/win32_monitor.c",
			"Phoenix/vendor/GLFW/src/win32_time.c",
			"Phoenix/vendor/GLFW/src/win32_thread.c",
			"Phoenix/vendor/GLFW/src/win32_window.c",
			"Phoenix/vendor/GLFW/src/wgl_context.c",
			"Phoenix/vendor/GLFW/src/egl_context.c",
			"Phoenix/vendor/GLFW/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"