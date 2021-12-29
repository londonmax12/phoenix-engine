#pragma once

#include "Phoenix/Project/Project.h"

#include <map>

namespace phx {
	struct RecentProject
	{
		std::string Name;
		std::string FilePath;
		time_t LastOpened;
	};

	struct UserPreferences
	{
		std::string StartupProject;
		std::map<time_t, RecentProject, std::greater<time_t>> RecentProjects;

		// Not Serialized
		std::string FilePath;
	};

}