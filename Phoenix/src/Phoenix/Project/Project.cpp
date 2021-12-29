#include "phxpch.h"
#include "Project.h"

namespace phx {
	void Project::SetCurrentScene(std::string scene)
	{
		m_CurrentScene = scene;
	}
	Ref<Project> Project::Create(std::string name, std::string path)
	{
		Project newProj = Project(name, path);
		Ref<Project> returnRef = CreateRef<Project>(newProj);
		return returnRef;
	}
}