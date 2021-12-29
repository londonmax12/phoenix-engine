#pragma once

#include <sstream>

#include "Phoenix/Application/Base.h"
#include "Phoenix/Events/Event.h"

namespace phx {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool Decorated;

		WindowProps(const std::string& title = "Phoenix Engine",
			uint32_t width = 1600,
			uint32_t height = 900,
			bool decorated = true)
			: Title(title), Width(width), Height(height), Decorated(decorated)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetWindowPos(int x, int y) = 0;
		virtual std::pair<int,int> GetWindowPos() = 0;

		virtual void MaximizeWindow() = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
};