#pragma once

#include "entt.hpp"

namespace phx {
	class Scene
	{
	public:
		Scene();
		~Scene();
	private:
		entt::registry m_Registry;

	};
}