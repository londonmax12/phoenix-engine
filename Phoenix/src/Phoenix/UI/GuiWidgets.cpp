#include "phxpch.h"

#include "GuiWidgets.h"

namespace phx {
	void UI::DrawItemRowsBackground(float lineHeight, const ImColor& color, const ImColor& color2)
	{
		{
			auto* drawList = ImGui::GetWindowDrawList();
			const auto& style = ImGui::GetStyle();

			if (lineHeight < 0)
			{
				lineHeight = ImGui::GetTextLineHeight();
			}
			lineHeight += style.ItemSpacing.y;

			float scrollOffsetH = ImGui::GetScrollX();
			float scrollOffsetV = ImGui::GetScrollY();
			float scrolledOutLines = floorf(scrollOffsetV / lineHeight);
			scrollOffsetV -= lineHeight * scrolledOutLines;

			ImVec2 clipRectMin(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
			ImVec2 clipRectMax(clipRectMin.x + ImGui::GetWindowWidth(), clipRectMin.y + ImGui::GetWindowHeight());

			if (ImGui::GetScrollMaxX() > 0)
			{
				clipRectMax.y -= style.ScrollbarSize;
			}

			drawList->PushClipRect(clipRectMin, clipRectMax);

			bool isOdd = (static_cast<int>(scrolledOutLines) % 2) == 0;

			float yMin = clipRectMin.y - scrollOffsetV + ImGui::GetCursorPosY();
			float yMax = clipRectMax.y - scrollOffsetV + lineHeight;
			float xMin = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMin().x;
			float xMax = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMax().x;

			for (float y = yMin; y < yMax; y += lineHeight, isOdd = !isOdd)
			{
				if (isOdd)
				{
					drawList->AddRectFilled({ xMin, y - style.ItemSpacing.y }, { xMax, y + lineHeight }, color);
				}
				else
				{
					drawList->AddRectFilled({ xMin, y - style.ItemSpacing.y }, { xMax, y + lineHeight }, color2);
				}
			}

			drawList->PopClipRect();
		}
	}
	bool UI::DrawButton(const std::string& label, ImVec2 size, float columnWidth)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();
		bool result = ImGui::Button(label.c_str(), size);

		ImGui::Columns(1);
		ImGui::PopID();
		ImGui::PopStyleVar();
		return result;
	}
	bool UI::DrawDragFloat(const std::string& label, float* value, float v_speed, float v_min, float v_max, const char* format, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		bool result = ImGui::DragFloat("##DragFloat", value, v_speed, v_min, v_max, format, 0, 25);
		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopID();

		return result;
		
	}
	bool UI::DrawDragFloat2(const std::string& label, float value[2], float v_speed, float v_min, float v_max, const char* format, float columnWidth)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();
		bool result = ImGui::DragFloat2("##DragFloat", value, v_speed, v_min, v_max, format);

		ImGui::Columns(1);
		ImGui::PopID();
		ImGui::PopStyleVar();

		return result;
	}
	bool UI::DrawCheckbox(const std::string& label, bool* value, float columnWidth)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();
		bool result = ImGui::Checkbox("##Checkbox", value);

		ImGui::Columns(1);
		ImGui::PopID();
		ImGui::PopStyleVar();
		return result;
	}
	void UI::DrawImage(Ref<Texture2D> image, ImVec2 size)
	{
		ImGui::Image((ImTextureID)image->GetRendererID(), size, { 0, 1 }, { 1, 0 });	
	}
	void UI::DrawVec2Controls(const std::string& label, glm::vec2& values, float resetValue, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.2f, 0.25f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);

		ImGui::PopStyleVar();
		ImGui::PopID();
	}
	void UI::DrawVec3Controls(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.2f, 0.25f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopStyleVar();
		ImGui::PopID();
	}
	void UI::DrawColorControls(const std::string& label, glm::vec4& values, float resetValue, float columnWidth)
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		static ImVec4 saved_palette[32] = {};
		if (saved_palette_init)
		{
			for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
			{
				ImGui::ColorConvertHSVtoRGB(n / 31.0f, 1.0f, 1.0f,
					saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
				saved_palette[n].w = 1.0f; // Alpha
			}
			saved_palette_init = false;
		}

		static ImVec4 backup_color;
		bool open_popup = ImGui::ColorButton("MyColor##3b", ImVec4{ values.x, values.y, values.z, values.w }, 0, ImVec2{ 25,25 });
		ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
		open_popup |= ImGui::Button("Palette", ImVec2{ 0, 25 });
		float col[4] = { values.x,values.y,values.z,values.w };
		if (open_popup)
		{
			ImGui::OpenPopup("mypicker");
			backup_color = ImVec4{ col[0], col[1], col[2], col[3] };
		}
		if (ImGui::BeginPopup("mypicker"))
		{
			ImGui::ColorPicker4("##picker", col, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaBar);
			ImGui::SameLine();

			ImGui::BeginGroup(); // Lock X position
			ImGui::Text("Current");
			ImGui::ColorButton("##current", ImVec4{ col[0], col[1], col[2], col[3] }, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip, ImVec2(60, 40));
			ImGui::Text("Previous");
			if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip, ImVec2(60, 40)))
			{
				col[0] = backup_color.x; // Preserve alpha!
				col[1] = backup_color.y;
				col[2] = backup_color.z;
				col[3] = backup_color.w;
			}

			ImGui::Separator();
			ImGui::Text("Palette");
			for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
			{
				ImGui::PushID(n);
				if ((n % 8) != 0)
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

				ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
				if (ImGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
				{
					col[0] = saved_palette[n].x;
					col[1] = saved_palette[n].y;
					col[2] = saved_palette[n].z;
					col[3] = saved_palette[n].w;
				}

				// Allow user to drop colors into each palette entry. Note that ColorButton() is already a
				// drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
						memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
						memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
					ImGui::EndDragDropTarget();
				}

				ImGui::PopID();
			}

			values.x = col[0];
			values.y = col[1];
			values.z = col[2];
			values.w = col[3];

			ImGui::EndGroup();
			ImGui::EndPopup();
		}
		ImGui::Columns(1);
	}
	void UI::DrawVec4ColorControls(const std::string& label, glm::vec4& values, float resetValue, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.2f, 0.25f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("R", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		float r = values.x * 255;
		if (ImGui::DragFloat("##R", &r, 0.1f, 0.0f, 255.0f, "%.0f"))
		{
			values.x = r / 255;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("G", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		float g = values.y * 255;
		if (ImGui::DragFloat("##G", &g, 2.0f, 0.0f, 255.0f, "%.0f"))
		{
			values.y = g / 255;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("B", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		float b = values.z * 255;
		if (ImGui::DragFloat("##B", &b, 2.0f, 0.0f, 255.0f, "%.0f"))
		{
			values.z = b / 255;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_PickerHueWheel;

		if (ImGui::ColorButton("##Picker", ImVec4{ values.x, values.y, values.z, values.w }, flags))
		{
			ImGui::OpenPopup("ColorPicker");
		}

		float col[4] = { values.x,values.y,values.z,values.w };

		if (ImGui::BeginPopup("ColorPicker"))
		{
			ImGui::PushFont(boldFont);
			ImGui::Text("Color Picker");
			ImGui::PopFont();
			ImGui::ColorPicker4("##ColorPicker", col, flags);
			ImGui::EndPopup();
		}

		values.x = col[0];
		values.y = col[1];
		values.z = col[2];
		values.w = col[3];

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}
	void UI::DrawGap()
	{	
		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::Separator();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();		
	}
}