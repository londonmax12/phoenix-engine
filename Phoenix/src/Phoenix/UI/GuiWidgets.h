#pragma once

#include "Phoenix/Renderer/Texture.h"

#include <string>

#include <imgui.h>
#include <imgui_internal.h>

#include <../vendor/glm/glm/glm.hpp>

namespace phx {
	static bool saved_palette_init = true;
	
	class UI {
	public:
		//static const float GapHeight = 1;
		static void PushHiddenButton();
		static void PopHiddenButton();

		static void DrawItemRowsBackground(float lineHeight = -1.0f, const ImColor& color = ImColor(20, 20, 20, 64), const ImColor& color2 = ImColor(30, 30, 30, 64));
		
		static bool DrawButton(const std::string& label, ImVec2 size = ImVec2(0, 0), float columnWidth = 100.0f);
		static bool DrawDragFloat(const std::string& label, float* value, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.2f", float columnWidth = 100.0f);
		static bool DrawDragFloat2(const std::string& label, float value[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.2f", float columnWidth = 100.0f);
		static bool DrawCheckbox(const std::string& label, bool* value, float columnWidth = 100.0f);
		static void DrawImage(Ref<Texture2D> image, ImVec2 size);
		static void DrawVec2Controls(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawVec3Controls(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawVec4ColorControls(const std::string& label, glm::vec4& values, float resetValue = 1.0f, float columnWidth = 100.0f);
		static void DrawColorControls(const std::string& label, glm::vec4& values, float resetValue = 1.0f, float columnWidth = 100.0f);
		
		static void DrawGap();
	};
}