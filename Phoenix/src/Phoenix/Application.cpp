#include "phxpch.h"
#include "Application.h"
#include "glfwWindow/Window.h"


namespace phx {
	Application::Application()
	{
		Window::PHX_Create_Window();
	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		while (true);
	}
}