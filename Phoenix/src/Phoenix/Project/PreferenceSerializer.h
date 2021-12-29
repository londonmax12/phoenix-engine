#pragma once

#include "Phoenix/Project/Preferences.h"

#include <filesystem>

namespace phx {
	class UserPreferencesSerializer
	{
	public:
		UserPreferencesSerializer(const Ref<UserPreferences>& preferences);
		~UserPreferencesSerializer();

		void Serialize(const std::filesystem::path& filepath);
		void Deserialize(const std::filesystem::path& filepath);

	private:
		Ref<UserPreferences> m_Preferences;
	};
}