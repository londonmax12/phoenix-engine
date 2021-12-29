#pragma once

#include "Project.h"

namespace phx {
	class ProjectSerializer
	{
	public:
		ProjectSerializer(const Ref<Project>& project);

		void Serialize(const std::string& filepath);
		Ref<Project> Deserialize(const std::string& filepath);
	private:
		Ref<Project> m_Project;
	};
}