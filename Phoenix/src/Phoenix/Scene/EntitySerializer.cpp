#include "phxpch.h"

#include "EntitySerializer.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

namespace phx {
	EntitySerializer::EntitySerializer(const Ref<Entity>& entity, const Ref<Scene>& scene)
		: m_Entity(entity), m_Scene(scene)
	{
	}
	void EntitySerializer::Serialize(const std::string& filepath)
	{
	}
	bool EntitySerializer::Deserialize(const std::string& filepath)
	{
		return false;
	}
}