project "Ezzoo-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/SandBox/Resources/Scripts")
	objdir ("%{wks.location}/SandBox/Resources/Scripts/Intermediates")

	files
	{
		"Source/**.cs",
		"Properties/**.cs",
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