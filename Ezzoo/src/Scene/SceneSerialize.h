#pragma once

#include "Scene.h"

#include "Ezzoo/Core/Core.h"


namespace Ezzoo {

	class SceneSerialize
	{
	public :
		SceneSerialize(const Ref <Scene>& scene);

		void Serialize(const std::string& filepath);
		bool SerializeRunTime(const std::string& filepath);


		bool Deserialize(const std::string& filepath);
		bool DeserializeRunTime(const std::string& filepath);

		static std::string OpenFile(const char* name);
		static std::string SaveFile(const char* name);

	private :

		Ref<Scene> m_Scene;



	};



}
