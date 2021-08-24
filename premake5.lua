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
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/GLAD/include",
		"%{prj.name}/vendor/ImGui",
		"%{prj.name}/vendor/Discord/include"
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"imgui",
		"Discord",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"PHX_PLATFORM_WINDOWS",
			"PHX_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PHX_DEBUG_MODE"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist_MODE"
		defines "PHX_DIST"
		buildoptions "/MD"
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
		staticruntime "Off"
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
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "PHX_DIST_MODE"
		buildoptions "/MD"
		optimize "On"


project "GLFW"
	location "vendor/GLFW"
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
		staticruntime "Off"
		
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

project "GLAD"
	location "vendor/GLAD"	
	kind "StaticLib"
	language "C"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "Phoenix/vendor/Glad/include/glad/glad.h",
        "Phoenix/vendor/Glad/include/KHR/khrplatform.h",
        "Phoenix/vendor/Glad/src/glad.c"
	}

	includedirs
	{
		"Phoenix/vendor/Glad/include"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "ImGui"
	location "vendor/imgui"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Phoenix/vendor/imgui/imconfig.h",
		"Phoenix/vendor/imgui/imgui.h",
		"Phoenix/vendor/imgui/imgui.cpp",
		"Phoenix/vendor/imgui/imgui_draw.cpp",
		"Phoenix/vendor/imgui/imgui_internal.h",
		"Phoenix/vendor/imgui/imgui_widgets.cpp",
		"Phoenix/vendor/imgui/imstb_rectpack.h",
		"Phoenix/vendor/imgui/imstb_textedit.h",
		"Phoenix/vendor/imgui/imstb_truetype.h",
		"Phoenix/vendor/imgui/imgui_demo.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
	
project "Discord"
	location "vendor/Discord"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Phoenix/vendor/Discord/include/discord_register.h",
		"Phoenix/vendor/Discord/include/discord_rpc.h",
		"Phoenix/vendor/Discord/src/backoff.h",
		"Phoenix/vendor/Discord/src/connection.h",
		"Phoenix/vendor/Discord/src/discord_rpc.cpp",
		"Phoenix/vendor/Discord/src/dllmain.cpp",
		"Phoenix/vendor/Discord/src/msg_queue.h",
		"Phoenix/vendor/Discord/src/rpc_connection.cpp",
		"Phoenix/vendor/Discord/src/rpc_connection.h",
		"Phoenix/vendor/Discord/src/serialization.cpp",
		"Phoenix/vendor/Discord/src/serialization.h"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		files 
		{
			"Phoenix/vendor/Discord/src/connection_win.cpp",
			"Phoenix/vendor/Discord/src/discord_register_win.cpp"
		}
		
		

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		files
		{
			"Phoenix/vendor/Discord/src/discord_register_linux.h"
		}
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
