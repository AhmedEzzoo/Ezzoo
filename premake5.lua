workspace "Ezzoo"
	architecture "x64"
	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Ezzoo/vendor/GLFW"
include "Ezzoo/vendor/Glad"
include "Ezzoo/vendor/IamGui"

project "Ezzoo"
	location "Ezzoo"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ezpch.h"
	pchsource "Ezzoo/src/ezpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	links 
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/IamGui"
	}

	filter "system:windows"
		defines
		{
			"EZZOO_WINDOWS_64",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "EZZOO_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EZZOO_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EZZOO_DIST"
		runtime "Release"
		optimize "on"



project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Ezzoo/vendor/spdlog/include",
		"Ezzoo/src",
		"Ezzoo/vendor"
		
	}

	links
	{
		"Ezzoo"
	}

	defines
	{
		"EZZOO_WINDOWS_64"
	}

	filter "configurations:Debug"
		defines "EZZOO_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EZZOO_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EZZOO_DIST"
		runtime "Release"
		optimize "on"