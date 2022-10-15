project "Phoenix-Script"
	location "Phoenix-Script"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	
	targetdir ("../Phoenix-Editor/resources/scripts")
	objdir ("../Phoenix-Editor/resources/scripts/int")

	files
	{
		"src/**.cs",
		"properties/**.cs"
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
	