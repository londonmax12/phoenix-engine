#pragma once
#include "Phoenix/Application/Window.h"
#include "Phoenix/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace phx {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void SetWindowPos(int x, int y) override;
		virtual std::pair<int, int> GetWindowPos() override;

		virtual void MaximizeWindow() override;

		virtual void* GetNativeWindow() const { return m_Window; };

		virtual std::pair<int, int> GetRelativeMousePos() override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		RenderContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		int X = 0;
		int Y = 0;

		WindowData m_Data;
	};

}