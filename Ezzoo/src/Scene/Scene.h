#pragma once

#include "entt.hpp"
#include "Ezzoo/Core/TimeStep.h"


namespace Ezzoo {

	class Entity;
	class SceneHierarchyPanel;
	class Scene
	{
	public :

		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdateEditor(TimeStep ts);
		void OnUpdateScript(TimeStep ts);


		void OnViewportResize(uint32_t width, uint32_t height);

	private :

		entt::registry m_Registry;
		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;


		friend class Entity;
		friend class SceneHierarchyPanel;


	};
}
