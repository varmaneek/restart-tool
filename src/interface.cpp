#include "restartHandler.h"
#include "interface.h"
#include "imgui.h"


void uiRenderer::renderUI(restartHandler& restartHandler) {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 250));

	ImGui::Begin("rt", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("Restart Hotkey: %s", restartHandler.getHotkeyName().c_str());
    ImGui::SameLine();
    
    if (restartHandler.isListeningForHotkey()) {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Press Any Key...");
    }
    else {
        if (ImGui::Button("Rebind")) {
            restartHandler.beginRebind();
        }
    }

    ImGui::InputText("Process Name", restartHandler.processNameBuffer, IM_ARRAYSIZE(restartHandler.processNameBuffer));

    ImGui::Separator();

    ImGui::Checkbox("Send Key After Restart", &restartHandler.sendKeyAfterRestart);
    ImGui::InputInt("Key Code To Send", &restartHandler.keyCodeToSend);
    ImGui::InputDouble("Additional Delay", &restartHandler.additionalDelay);

	ImGui::End();
}

void uiRenderer::applyStyle() {
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    ImVec4 backgroundColor = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
    ImVec4 surfaceColor = ImVec4(0.16f, 0.16f, 0.19f, 1.00f);
    ImVec4 accentColor = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    ImVec4 accentHover = ImVec4(0.36f, 0.69f, 1.00f, 1.00f);
    ImVec4 textColor = ImVec4(0.92f, 0.92f, 0.94f, 1.00f);

    style.Colors[ImGuiCol_WindowBg] = backgroundColor;
    style.Colors[ImGuiCol_ChildBg] = backgroundColor;
    style.Colors[ImGuiCol_PopupBg] = surfaceColor;
    style.Colors[ImGuiCol_Text] = textColor;
    style.Colors[ImGuiCol_FrameBg] = surfaceColor;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_Button] = accentColor;
    style.Colors[ImGuiCol_ButtonHovered] = accentHover;
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.45f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = accentColor;
    style.Colors[ImGuiCol_SliderGrab] = accentColor;
    style.Colors[ImGuiCol_SliderGrabActive] = accentHover;
    style.Colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 0.50f);
    
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowBorderSize = 0.0f;
}