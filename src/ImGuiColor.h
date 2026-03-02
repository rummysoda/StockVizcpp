//
// Created by rummy on 1/3/2026.
//

#ifndef IMGUICOLOR_H
#define IMGUICOLOR_H
#include "imgui.h"

inline void SetupImGuiStyle(ImVec4 baseCol, float alpha_ = 1.0f) {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha = alpha_;
    style.FrameRounding = 3.0f;

    float h, s, v;
    ImGui::ColorConvertRGBtoHSV(baseCol.x, baseCol.y, baseCol.z, h, s, v);

    auto col = [h](float s, float v, float a) {
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
        return ImVec4(r, g, b, a);
    };

    style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = col(0.10f, 0.10f, 0.94f);
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]               = col(0.10f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border]                = col(0.30f, 0.30f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = col(0.40f, 0.20f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered]        = col(0.60f, 0.40f, 0.67f);
    style.Colors[ImGuiCol_FrameBgActive]         = col(0.80f, 0.50f, 0.67f);
    style.Colors[ImGuiCol_TitleBg]               = col(0.30f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = col(0.50f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = col(0.20f, 0.10f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg]             = col(0.20f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]           = col(0.10f, 0.06f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = col(0.30f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = col(0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = col(0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = col(0.80f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = col(0.60f, 0.60f, 0.88f);
    style.Colors[ImGuiCol_SliderGrabActive]      = col(0.80f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_Button]                = col(0.60f, 0.40f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]         = col(0.70f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = col(0.80f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_Header]                = col(0.50f, 0.40f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]         = col(0.60f, 0.60f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]          = col(0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_Separator]             = col(0.30f, 0.30f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]      = col(0.60f, 0.60f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]       = col(0.80f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = col(0.40f, 0.30f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = col(0.60f, 0.60f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]      = col(0.80f, 0.80f, 0.95f);
    style.Colors[ImGuiCol_Tab]                   = col(0.40f, 0.30f, 0.58f);
    style.Colors[ImGuiCol_TabHovered]            = col(0.60f, 0.60f, 0.80f);
    style.Colors[ImGuiCol_TabActive]             = col(0.50f, 0.50f, 0.68f);
    style.Colors[ImGuiCol_PlotLines]             = col(0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = col(0.80f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = col(0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = col(0.90f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = col(0.60f, 0.50f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}
#endif //IMGUICOLOR_H
