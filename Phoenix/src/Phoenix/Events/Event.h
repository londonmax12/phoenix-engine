#pragma once

#include "Core.h"

namespace phx {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResized, WindowFocused, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		Event_Window = BIT(0),
		Event_Input = BIT(1),
		Event_Keyboard = BIT(2),
		Event_Mouse = BIT(3),
		Event_MouseInput = BIT(4)
	};
}

/*#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\ 
virtual EventType GetEventType() const override{return GetStaticType();}\
Virtual const char* GetName() const override{return #type;}

