#include <Phoenix.h>
#include "Phoenix/Application/EntryPoint.h"

#include "EditorLayer.h"


namespace phx {
	class PhoenixEditor : public phx::Application
	{
	public:
		PhoenixEditor()
			: Application("Phoenix Editor")
		{
			PushLayer(new EditorLayer());
		}
		~PhoenixEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new PhoenixEditor();
	}
}
