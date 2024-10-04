#pragma once

#include "entt.hpp"
#include "Ezzoo/Core/TimeStep.h"
#include "Ezzoo/Renderer/EditorCamera.h"
#include "Ezzoo/Core/UUID.h"

class b2World;

namespace Ezzoo {


	class Entity;
	
	class Scene
	{
	public :

		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithID(const UUID& id, const std::string& name);

		static Ref<Scene> Copy(Ref<Scene> other);
		Entity DuplicateEntity(Entity src);

		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
		void OnUpdateRunTime(TimeStep ts);
		void OnUpdateScript(TimeStep ts);
		void OnUpdateSimulate(TimeStep ts, EditorCamera& camera);


		void DestroyEntity(Entity entity);

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetCameraPrimaryEntity();

		void OnRunTimeStart();
		void OnRunTimeStop();
		void OnSimulationStart();
		void OnSimulationStop();

		void OnPhysics2DRender();

		template <typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}


	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);
	private :

		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0 , m_ViewportHeight = 0;
		
		b2World* m_Physicsworld = nullptr;

		friend class Entity;
		friend class SceneSerialize;
		friend class SceneHierarchyPanel;


	};
}
