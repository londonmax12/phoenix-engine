workspace "Phoenix"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	project "GLFW"
		location "vendor/GLFW"
		kind "StaticLib"
		language "C"
		staticruntime "on"
	
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
			systemversion "latest"
		
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
		filter "configurations:Debug"
			defines "PHX_DEBUG_MODE"
			buildoptions "/MDd"
			symbols "on"

		filter "configurations:Release"
			defines "PHX_RELEASE_MODE"
			buildoptions "/MD"
			optimize "on"
		filter "configurations:Dist"
			defines "PHX_DIST_MODE"
			buildoptions "/MD"
			optimize "on"

	project "Glad"
		location "vendor/Glad"
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
			"include"
		}		
		filter "system:windows"
			systemversion "latest"
	
		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
			buildoptions "/MDd"
		filter "configurations:Release"
			runtime "Release"
			optimize "on"
			buildoptions "/MD"

	project "ImGui"
		location "vendor/imgui"
		kind "StaticLib"
		language "C++"
		staticruntime "on"

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

		defines { "IMGUI_API=__declspec(dllexport)" }
	
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
			buildoptions "/MDd"
		filter "configurations:Release"
			runtime "Release"
			optimize "on"
			buildoptions "/MD"
		filter "configurations:Dist"
			runtime "Release"
			optimize "on"
			buildoptions "/MD"

	project "Discord"
		location "vendor/Discord"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

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
			buildoptions "/MDd"
		filter "configurations:Release"
			runtime "Release"
			optimize "on"
			buildoptions "/MDd"
		filter "configurations:Dist"
			runtime "Release"
			optimize "on"
			buildoptions "/MDd"

	project "yaml-cpp"
		location "vendor/yaml-cpp"
		kind "StaticLib"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"Phoenix/vendor/yaml_cpp/src/**.h",
			"Phoenix/vendor/yaml_cpp/src/**.cpp",
		
			"Phoenix/vendor/yaml_cpp/include/**.h"
		}

		includedirs
		{
			"Phoenix/vendor/yaml_cpp/include/"
		}

		filter "system:windows"
			systemversion "latest"
			cppdialect "C++17"
			staticruntime "On"

		filter "system:linux"
			pic "On"
			systemversion "latest"
			cppdialect "C++17"
			staticruntime "On"

		filter "configurations:Debug"
			buildoptions "/MDd"
			runtime "Debug"
			symbols "on"
		filter "configurations:Release"
			buildoptions "/MD"
			runtime "Release"
			optimize "on"
		filter "configurations:Dist"
			buildoptions "/MD"
			runtime "Release"
			optimize "on"

	project "Box2D"
		kind "StaticLib"
		language "C++"
		location "vendor/Box2D"
		cppdialect "C++11"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"Phoenix/vendor/box2d/src/**.h",
			"Phoenix/vendor/box2d/src/**.cpp",
			"Phoenix/vendor/box2d/include/**.h"
		}

		includedirs
		{
			"Phoenix/vendor/box2d/include",
			"Phoenix/vendor/box2d/src"
		}

		filter "system:windows"
			systemversion "latest"

		filter "configurations:Debug"
			runtime "Debug"
			buildoptions "/MDd"
			symbols "on"

		filter "configurations:Release"
			runtime "Release"
			buildoptions "/MD"
			optimize "on"

		filter "configurations:Dist"
			runtime "Release"
			buildoptions "/MD"
			optimize "on"
group ""

project "Phoenix"
	location "Phoenix"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "phxpch.h"
	pchsource "Phoenix/src/phxpch.cpp"
	
	VULKAN_SDK = os.getenv("VULKAN_SDK")
	MONO_PROJECT = os.getenv("MONO_PROJECT")

	LibraryDir = {}

	LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

	Library = {}
	Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
	Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

	Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
	Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
	Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/vendor/imgui/examples/imgui_impl_opengl3.h",
		"%{prj.name}/vendor/imgui/examples/imgui_impl_opengl3.cpp",
		"%{prj.name}/vendor/imgui/examples/imgui_impl_glfw.h",
		"%{prj.name}/vendor/imgui/examples/imgui_impl_glfw.cpp",
		"%{prj.name}/vendor/ImGuiColorTextEdit/TextEditor.cpp",
		"%{prj.name}/vendor/ImGuiColorTextEdit/TextEditor.h"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/GLAD/include",
		"%{prj.name}/vendor/ImGui",
		"%{prj.name}/vendor/Discord/include",
		"%{prj.name}/vendor/OpenAL/include",
		"%{prj.name}/vendor/libsndfile/",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/stb_image",
		"%{prj.name}/vendor/entt/include",
		"%{prj.name}/vendor/yaml_cpp/include",
		"%{prj.name}/vendor/box2d/include",
		"%{prj.name}/vendor/ImGuizmo",
		"VulkanSDK/{%{VULKAN_SDK}/include",
		"%{MONO_PROJECT}/include/mono-2.0"
	}

	libdirs
	{
		"Phoenix/libs",
		"%{MONO_PROJECT}/lib"
	}

	links 
	{ 
		"Box2D",
		"GLFW",
		"GLAD",
		"imgui",
		"Discord",
		"yaml-cpp",


		"mono-2.0-sgen.lib",

		"sndfile.lib",
		"OpenAL32.lib",

		"opengl32.lib"
	}

	filter "files:vendor/ImGuizmo/ImGuizmo.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{		
			"_CRT_SECURE_NO_WARNINGS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "PHX_DEBUG_MODE"
		buildoptions "/MDd"
		symbols "on"
		libdirs
		{
			"%{prj.name}/vendor/VulkanSDK/Lib/"
		}
		links
		{
			"shaderc_sharedd.lib",
			"spirv-cross-cored.lib",
			"spirv-cross-glsld.lib",
			"SPIRV-Toolsd.lib",
		}

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		buildoptions "/MD"
		optimize "on"
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}		

	filter "configurations:Dist"
		defines "PHX_DIST_MODE"
		buildoptions "/MD"
		optimize "on"
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
project "Sandbox" 
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"Phoenix/vendor/glm",
		"Phoenix/vendor/imgui",
		"Phoenix/src",
		"Phoenix/vendor/entt/include"
	}

	links
	{
		"Phoenix"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines 
		{
			"PHX_DEBUG_MODE",
			"PHX_ENABLE_ASSERTS"
		}
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "PHX_DIST_MODE"
		buildoptions "/MD"
		optimize "on"


project "Phoenix-Editor" 
	location "Phoenix-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	MONO_PROJECT = os.getenv("MONO_PROJECT")


	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Phoenix/src",
		"Phoenix/vendor",
		"Phoenix/vendor/spdlog/include",
		"Phoenix/vendor/glm",
		"Phoenix/vendor/imgui",
		"Phoenix/vendor/entt/include",
		"Phoenix/vendor/ImGuizmo",
		"%{MONO_PROJECT}/include/mono-2.0"
	}

	links
	{
		"Phoenix"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines 
		{
			"PHX_DEBUG_MODE",
			"PHX_ENABLE_ASSERTS"
		}
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "PHX_RELEASE_MODE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "PHX_DIST_MODE"
		buildoptions "/MD"
		optimize "on"
