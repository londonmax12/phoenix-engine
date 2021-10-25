#pragma once

#include "Phoenix/Layer/Layer.h"

#include "Phoenix/Events/KeyboardEvent.h"
#include "Phoenix/Events/MouseEvent.h"
#include "Phoenix/Events/WindowEvent.h"

namespace phx {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}