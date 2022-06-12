#pragma once

#include "Phoenix/Scene/Entity.h"

namespace phx {
	class EntitySerializer
	{
	public:
		EntitySerializer(const Ref<Entity>& entity, const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
	private:
		Ref<Entity> m_Entity;
		Ref<Scene> m_Scene;
	};
}