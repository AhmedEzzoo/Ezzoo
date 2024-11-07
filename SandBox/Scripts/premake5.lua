workspace"Sandbox"
	architecture "x64"
	configurations {"Debug", "Release", "Dist"}
	startproject "Sandbox"

	flags
	{
		"MultiProcessorCompile"
	}


include "../../Ezzoo-ScriptCore"


project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Resources/Scripts")
	objdir ("Resources/Intermediates")

	files
	{
		"Sources/**.cs",
	}

	links
	{
		"Ezzoo-ScriptCore"
	}

	filter "configurations:Debug"
		optimize "off"
		symbols "Default"

	filter "configurations:Release"
		optimize "on"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "off"