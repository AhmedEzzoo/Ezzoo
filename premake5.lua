workspace "Ezzoo"
	architecture "x64"
	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Ezzoo/vendor/GLFW"
include "Ezzoo/vendor/Glad"
include "Ezzoo/vendor/imgui"
include "Ezzoo/vendor/yaml-cpp"
include "Ezzoo/vendor/Box2D"

project "Ezzoo"
	location "Ezzoo"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ezpch.h"
	pchsource "Ezzoo/src/ezpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		--"%{prj.name}/vendor/entt/include/entt.hpp",
		
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo/ImGuizmo.cpp"


	}

	links 
	{
		"GLFW",
		"Glad",
		"imgui",
		"yaml-cpp",
		"Box2D",
		"opengl32.lib"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_DEFINE_MATH_OPERATORS"
	}


	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/stb_image",
		"%{prj.name}/vendor/entt/include",
		"%{prj.name}/vendor/yaml-cpp/include",
		"%{prj.name}/vendor/Box2D/include",
		"D:/C++ visual studio/Vulkan/Include",

		"%{prj.name}/vendor/ImGuizmo"
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
		links
		{
			"D:/C++ visual studio/Vulkan/Lib/shaderc_sharedd.lib",
			"D:/C++ visual studio/Vulkan/Lib/spirv-cross-cored.lib",
			"D:/C++ visual studio/Vulkan/Lib/spirv-cross-glsld.lib",
			--"D:/C++ visual studio/Vulkan/Lib/SPIRV-Tools.lib"
		}


	filter "configurations:Release"
		defines "EZZOO_RELEASE"
		runtime "Release"
		optimize "on"
		links
		{
			"D:/C++ visual studio/Vulkan/Lib/shaderc_shared.lib",
			"D:/C++ visual studio/Vulkan/Lib/spirv-cross-core.lib",
			"D:/C++ visual studio/Vulkan/Lib/spirv-cross-glsl.lib",
			--"D:/C++ visual studio/Vulkan/Lib/SPIRV-Toolsd.lib"
		}

	filter "configurations:Dist"
		defines "EZZOO_DIST"
		runtime "Release"
		optimize "on"

		
	filter "files:Ezzoo/vendor/ImGuizmo/**.cpp"
	flags {"NoPCH"}



project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	LibraryDir = {}

	LibraryDir["VulkanSDK_DebugDLL"] = "D:/C++ visual studio/Vulkan/Bin"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		--"Ezzoo/vendor/entt/include/entt.hpp"
	}
	
	includedirs
	{
		"Ezzoo/src",
		--"Ezzoo/vendor/yaml-cpp/include",
		"Ezzoo/vendor/spdlog/include",
		"Ezzoo/vendor",
		"Ezzoo/vendor/glm",
		"Ezzoo/vendor/entt/include",
		"Ezzoo/vendor/imgui",
		"Ezzoo/vendor/ImGuizmo",
		--"Ezzoo/vendor/Box2D/include"


	}

	links
	{
		"Ezzoo",
		--"yaml-cpp"
	}

	defines
	{
		"EZZOO_WINDOWS_64"
	}

	filter "configurations:Debug"
		defines "EZZOO_DEBUG"
		runtime "Debug"
		symbols "on"

		--postbuildcommands
		--{
		--	"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
		--}


	filter "configurations:Release"
		defines "EZZOO_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EZZOO_DIST"
		runtime "Release"
		optimize "on"