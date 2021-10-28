#pragma once

#include "entt.hpp"
#include "Phoenix/Time/DeltaTime.h"


namespace phx {
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(DeltaTime dt);
	private:
		entt::registry m_Registry;
	};
}