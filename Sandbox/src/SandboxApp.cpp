#include <Phoenix.h>

class ExampleLayer : public phx::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

};

class Sandbox : public phx::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

phx::Application* phx::CreateApplication()
{
	return new Sandbox();
}