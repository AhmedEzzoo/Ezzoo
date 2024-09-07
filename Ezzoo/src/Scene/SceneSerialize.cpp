#include "ezpch.h"

#include "SceneSerialize.h"

#include <yaml-cpp/yaml.h>

namespace Ezzoo {

	SceneSerialize::SceneSerialize(const Ref <Scene>& scene) 
		: m_Scene(scene)
	{
	}

	void SceneSerialize::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		YAML::BeginMap;



		YAML::EndMap;
	}

	bool SceneSerialize::SerializeRunTime(const std::string& filepath)
	{
		return false;
	}

	void SceneSerialize::Deserialize(const std::string& filepath)
	{

	}

	bool SceneSerialize::DeserializeRunTime(const std::string& filepath)
	{
		EZZOO_CORE_ASSERT(!m_Scene, "Scene Doesnot Exist");

		return false;
	}

}