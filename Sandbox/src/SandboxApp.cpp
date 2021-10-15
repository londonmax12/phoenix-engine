#include <Phoenix.h>

#include "../../Phoenix/vendor/imgui/imgui.h"

class ExampleLayer : public phx::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}
	void OnEvent(phx::Event& event) override
	{
		if (phx::Input::IsKeyPressed(PHX_KEY_TAB))
			PHX_CORE_INFO(phx::Input::IsKeyPressed(PHX_KEY_TAB));
	}
	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Test Window");
		ImGui::End();
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