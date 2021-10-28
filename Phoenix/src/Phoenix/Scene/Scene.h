#pragma once
#include "entt.hpp"
#include "Phoenix/Time/DeltaTime.h"


namespace phx {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string name = std::string());

		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(DeltaTime dt);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}