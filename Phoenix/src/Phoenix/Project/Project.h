#pragma once

#include "Phoenix/Scene/Scene.h"

namespace phx {
	class Project {
	public:
		Project(std::string name, std::string path)
			: m_Name(name), m_Path(path) {}
		void SetCurrentScene(std::string scene);

		static Ref<Project> Create(std::string name, std::string path);

		std::string m_CurrentScene{};
		std::string m_Path;
		std::string m_AssetPath;
		std::string m_Name;
	private:
	};
}