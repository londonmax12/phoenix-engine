#include "phxpch.h"
#include "Phoenix/Application/Base.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace phx {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		PHX_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		PHX_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PHX_CORE_ASSERT(status, "Failed to initialize Glad!");
		PHX_CORE_INFO("OpenGL Renderer:");
		PHX_CORE_INFO("   - Vendor: {0}", glGetString(GL_VENDOR));
		PHX_CORE_INFO("   - Renderer: {0}", glGetString(GL_RENDERER));
		PHX_CORE_INFO("   - Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		PHX_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}