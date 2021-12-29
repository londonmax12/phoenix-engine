#include <Phoenix.h>
#include "Phoenix/Application/EntryPoint.h"

#include "EditorLayer.h"


namespace phx {
	class PhoenixEditor : public phx::Application
	{
	public:
		PhoenixEditor(ApplicationCommandLineArgs args)
			: Application(ApplicationSpecification("Phoenix Editor", true, 1600, 900, true), args)
		{
			PushLayer(new EditorLayer());
		}
		~PhoenixEditor()
		{

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new PhoenixEditor(args);
	}
}
