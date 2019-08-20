#include "ui_manager.h"

#include "imgui.h"
#include "logic_manager.h"

ImVec2 UIManager::getSDLWindowSize() const {
	int width;
	int height;
	SDL_GetWindowSize(SDLwindow, &width, &height);
	return {static_cast<float>(width), static_cast<float>(height)};
}

void UIManager::render(const LogicManager& logicState) {
	// static bool active = true;
	// ImGui::Begin("Heeelloooo", &active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_);
	// ImGui::Text("hm");
	// ImGui::End();

	renderCosmos(logicState);
}

void UIManager::renderCosmos(const LogicManager& logicState) const {
	static bool cosmosOpen = true;
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize(getSDLWindowSize());
	ImGui::Begin("Cosmos", &cosmosOpen, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddCircle({500, 200}, 100.0, ImColor(0.5f, 0.5f, 0.5f, 1.0f));

	ImGui::End();
}