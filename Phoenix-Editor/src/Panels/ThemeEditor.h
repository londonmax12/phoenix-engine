#pragma once

#include <vector>
#include <imgui/imgui.h>
#include <Windows.h>
#include <string>
#include <fstream>
#include <commdlg.h>

std::vector<std::string> theme;

ImVec4 gridbgcol;
ImVec4 gridlinecol;

std::string savethemefilename() {
    OPENFILENAME ofn;
    char fileName[256] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = (LPCWSTR)L"r6theme Files (*.r6theme)\0*.r6theme\0";
    ofn.lpstrFile = (LPWSTR)fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"r6theme";

    GetSaveFileName(&ofn);;
    return fileName;
}

void renderThemeEditor()
{
    ImGuiStyle& style = ImGui::GetStyle();
    if (ImGui::Begin("Themes"))
    {
        static int output_dest = 0;
        static bool output_only_modified = true;
        if (ImGui::Button("Export"))
        {
            std::string filename = savethemefilename();
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const ImVec4 col = style.Colors[i];
                const char* name = ImGui::GetStyleColorName(i);
                theme.push_back("style.Colors[ImGuiCol_" + std::string(name) + "] = ImVec4(" + std::to_string(col.x) + "f," + std::to_string(col.y) + "f," + std::to_string(col.z) + "f," + std::to_string(col.w) + ");");
            }
            std::ofstream outfile;
            outfile.open(filename);
            outfile << "auto& style = ImGui::GetStyle();" << std::endl;
            outfile << "style.WindowMenuButtonPosition = ImGuiDir_None;" << std::endl;
            for (std::string line : theme)
            {
                outfile << line << std::endl;
            }
            theme.clear();
            outfile.close();
        }
        ImGui::SameLine();
        static ImGuiTextFilter filter;
        filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

        static ImGuiColorEditFlags alpha_flags = 0;
        if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
        if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
        if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } ImGui::SameLine();
        ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
        ImGui::PushItemWidth(-160);
        ImGui::NewLine();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName(i);
            if (!filter.PassFilter(name))
                continue;
            ImGui::PushID(i);
            ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);

            ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
            ImGui::TextUnformatted(name);
            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
    ImGui::End();
    
}
