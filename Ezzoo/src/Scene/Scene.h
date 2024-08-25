#pragma once

#include "entt.hpp"
#include "Ezzoo/Core/TimeStep.h"


namespace Ezzoo {

	class Entity;

	class Scene
	{
	public :

		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdateEditor(TimeStep ts);

	private :

		entt::registry m_Registry;
		
		friend class Entity;
	};
}
