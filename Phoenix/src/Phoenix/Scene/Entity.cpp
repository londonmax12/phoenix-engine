#include "phxpch.h"
#include "Entity.h"

namespace phx {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
}