#include <Phoenix.h>

class ExampleLayer : public phx::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnEvent(phx::Event& event) override
	{

	}


};

class Sandbox : public phx::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new phx::ImGuiLayer());
	}
	~Sandbox()
	{

	}
};

phx::Application* phx::CreateApplication()
{
	return new Sandbox();
}