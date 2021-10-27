#pragma once

#include "Phoenix/Input/Keycodes.h"
#include "Phoenix/Input/Mousecodes.h"

#include "Phoenix/Application/Base.h"

namespace phx {
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		virtual ~Input() = default;
	protected:
		Input() = default;
	private:
		static Input* s_Instance;
	};
}