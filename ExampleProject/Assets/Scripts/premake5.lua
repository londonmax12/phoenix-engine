local PhoenixRootDir = '../../..'

workspace "Sandbox"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("bin")
	objdir ("bin-int")

	files 
	{
		"src/**.cs",
		"props/**.cs"
	}

	links
	{
		"Phoenix-Script"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group "Phoenix"
	include (PhoenixRootDir .. "/Phoenix-Script")
group ""