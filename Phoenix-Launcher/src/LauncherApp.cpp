#include <Phoenix.h>
#include "Phoenix/Application/EntryPoint.h"

#include "LauncherLayer.h"


namespace phx {
	class PhoenixLauncher : public phx::Application
	{
	public:
		PhoenixLauncher(ApplicationCommandLineArgs args)
			: Application(ApplicationSpecification("Phoenix Launcher", false, 1000, 600, false), args)
		{
			PushLayer(new LauncherLayer());
		}
		~PhoenixLauncher()
		{

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new PhoenixLauncher(args);
	}
}
