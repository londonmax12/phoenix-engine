#include <Phoenix.h>
#include "Phoenix/Application/EntryPoint.h"
#include <Phoenix/Project/PreferenceSerializer.h>
#include <Phoenix/Utils/Environment.h>

#include "LauncherLayer.h"
#include <ShlObj.h>



namespace phx {
	class PhoenixLauncher : public phx::Application
	{
	public:
		PhoenixLauncher(ApplicationCommandLineArgs args)
			: Application(ApplicationSpecification("Phoenix Launcher", false, 1000, 600, false, false), args)
		{
			{
				PWSTR roamingFilePath;
				HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, NULL, &roamingFilePath);
				if (result == S_OK)
				{
					std::wstring filepath = roamingFilePath;
					std::replace(filepath.begin(), filepath.end(), L'\\', L'/');
					m_PersistentStoragePath = filepath + L"/Phoenix-Editor";

					if (!std::filesystem::exists(m_PersistentStoragePath))
						std::filesystem::create_directory(m_PersistentStoragePath);
				}	
			}
			// User Preferences
			{
				std::filesystem::path path = m_PersistentStoragePath / "UserPreferences.yaml";
				UserPreferencesSerializer serializer(m_UserPreferences);
				if (!std::filesystem::exists(path))
					serializer.Serialize(path);
				else
					serializer.Deserialize(path);
				m_UserPreferences->FilePath = path.string();
			}
			LauncherProperties launcherProperties;
			launcherProperties.UserPreferences = m_UserPreferences;
			{
				if (Environment::HasEnvironmentalVariable("PHOENIX_DIR"))
					launcherProperties.InstallPath = Environment::GetEnvironmentalVariable("PHOENIX_DIR");
			}

			PushLayer(new LauncherLayer(launcherProperties));
		}
		~PhoenixLauncher()
		{

		}
	private:
		std::filesystem::path m_PersistentStoragePath;
		Ref<UserPreferences> m_UserPreferences = CreateRef<UserPreferences>(UserPreferences());
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new PhoenixLauncher(args);
	}
}
