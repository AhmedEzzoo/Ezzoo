workspace "Ezzoo"
	architecture "x64"
	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Ezzoo/vendor/GLFW"
include "Ezzoo/vendor/Glad"
include "Ezzoo/vendor/ImGuiDock"

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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"

	}
	links 
	{
		"GLFW",
		"Glad",
		"ImGuiDock",
		"opengl32.lib"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/ImGuiDock",
		"%{prj.name}/vendor/glm"
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
		"Ezzoo/src",
		"Ezzoo/vendor/spdlog/include",
		"Ezzoo/vendor",
		"Ezzoo/vendor/glm"
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