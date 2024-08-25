
#include "ezpch.h"
#include "Entity.h"

namespace Ezzoo {

	Entity::Entity(entt::entity entity, Scene* scene)
		: m_Entity(entity), m_Scene(scene)
	{}
}