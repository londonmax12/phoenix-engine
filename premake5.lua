--premake.lua

workspace "Phoenix"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distrabution"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Phoenix"
	location "Phoenix"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" ..outputDir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputDir.. "/%{prj.name}")

	pchheader "phxpch.h"
	pchsource "Phoenix/src/phxpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/src/Phoenix",
		"%{prj.name}/src"
	}

    libdirs
    {
        "%{prj.name}/vendor/glfw/lib-vc2019"
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
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputDir.. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PHX_DEBUG_MODE"
		symbols "On"

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		symbols "On"

	filter "configurations:Distrabution"
		defines "PHX_DIST_MODE"
		symbols "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"

		language "C++"

	targetdir ("bin/" ..outputDir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputDir.. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Phoenix/vendor/spdlog/include",
		"Phoenic/vendor/glfw/include",
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
		defines "PHX_DEBUG_MODE"
		symbols "On"

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		symbols "On"

	filter "configurations:Distrabution"
		defines "PHX_DIST_MODE"
		symbols "On"