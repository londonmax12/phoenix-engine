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

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}