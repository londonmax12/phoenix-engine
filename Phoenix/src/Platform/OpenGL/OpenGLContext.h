#pragma once
#include "Phoenix/Renderer/Renderer.h"

struct GLFWwindow;

namespace phx {
	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private: 
		GLFWwindow* m_WindowHandle;
	};
}