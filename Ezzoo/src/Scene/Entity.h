#pragma once

#include "Scene.h"

namespace Ezzoo {

	class Entity
	{

	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		~Entity() = default;


		template <typename T, typename ...ARGS>
		T& AddComponent(ARGS &&...args)
		{
			T& component = m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<ARGS>(args)...);

			return component;
		}

		template <typename T>
		T& GetComponent()
		{ 
			EZZOO_CORE_ASSERT(HasComponent<T>(), "Requested Component is Not Available");
			return m_Scene->m_Registry.get<T>(m_Entity);


		}



		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_Entity);
		}


		template <typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_Entity);
		}

		operator uint32_t()
		{
			return (uint32_t)m_Entity;
		}

	private :

		entt::entity m_Entity = entt::null;
		Scene* m_Scene = nullptr;

	};
}