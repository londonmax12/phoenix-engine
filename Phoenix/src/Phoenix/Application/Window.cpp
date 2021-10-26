#include "phxpch.h"
#include "Phoenix/Application/Window.h"

#ifdef PHX_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace phx
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef PHX_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		PHX_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}