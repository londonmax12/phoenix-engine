#include "phxpch.h"
#include "WindowsWindow.h"

#include "Phoenix/Events/KeyboardEvent.h"
#include "Phoenix/Events/WindowEvent.h"
#include "Phoenix/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLAD/glad.h>

namespace phx {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		PHX_CORE_ERROR("GLFW Error ({0}: {1})", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		PHX_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		PHX_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		PHX_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Width = props.Height;

		PHX_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		glfwWindowHint(GLFW_SAMPLES, 16);
		if (!s_GLFWInitialized)
		{
			PHX_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			PHX_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		if (!props.Decorated)
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		{
			PHX_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);

		int monitorX, monitorY;
		glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);
		X = monitorX + (videoMode->width / 2) - (props.Width / 2);
		Y = monitorY + (videoMode->height / 2) - (props.Height / 2);

		glfwSetWindowPos(m_Window, X, Y);

		// Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			}
		);
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			}
		);
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}								
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}					
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}			
				}
			}
		);
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					phx::MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					phx::MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			}
		);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				phx::MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			}
		);
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				phx::MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			}
		);
	}

	void WindowsWindow::Shutdown()
	{
		PHX_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		PHX_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		PHX_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::SetWindowPos(int x, int y)
	{
		glfwSetWindowPos(m_Window, x, y);
		X = x;
		Y = y;
	}

	std::pair<int, int> WindowsWindow::GetWindowPos()
	{
		return std::pair<int, int>(X,Y);
	}
	void WindowsWindow::MaximizeWindow()
	{
		//glfwMaximizeWindow(m_Window);
	}
}