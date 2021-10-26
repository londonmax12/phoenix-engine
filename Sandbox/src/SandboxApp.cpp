#include <Phoenix.h>
#include "Phoenix/Application/EntryPoint.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class Sandbox : public phx::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{

	}
	void OnInit() override {
		
	}
};

phx::Application* phx::CreateApplication()
{
	return new Sandbox();
}