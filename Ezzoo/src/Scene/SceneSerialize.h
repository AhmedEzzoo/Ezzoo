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


		void Deserialize(const std::string& filepath);
		bool DeserializeRunTime(const std::string& filepath);



	private :

		Ref<Scene> m_Scene;



	};



}
