#include "ui_manager.h"

#include <algorithm>
#include <iostream>

#include "imgui.h"
#include "logic_manager.h"

Vec2 UIManager::getSDLWindowSize() const {
	int width;
	int height;
	SDL_GetWindowSize(SDLwindow, &width, &height);
	return { static_cast<double>(width), static_cast<double>(height) };
}

void UIManager::render(LogicManager& logicState, int wheelEvent) {
	renderCosmos(logicState, wheelEvent);

	static bool active = true;
	ImGui::Begin("Something", &active);
	ImGui::Text("Hello");
	ImGui::End();
}

void UIManager::setZoom(double newZoom) {
	cameraZoom = newZoom;
}
void UIManager::setZoom(double newZoom, Vec2 anchor) {
	Vec2 windowSize = getSDLWindowSize();
	Vec2 windowCenter = { windowSize.x / 2, windowSize.y / 2 };

	Vec2 oldPos = {
		((cameraPosition.x * cameraZoom) + (anchor.x - windowCenter.x)) / cameraZoom,
		((cameraPosition.y * cameraZoom) + (anchor.y - windowCenter.y)) / cameraZoom
	};
	Vec2 newPos = {
		((cameraPosition.x * newZoom) + (anchor.x - windowCenter.x)) / newZoom,
		((cameraPosition.y * newZoom) + (anchor.y - windowCenter.y)) / newZoom
	};
	cameraPosition.x += oldPos.x - newPos.x;
	cameraPosition.y += oldPos.y - newPos.y;
	cameraZoom = newZoom;
}

void UIManager::renderCosmos(LogicManager& logicState, int wheelEvent) {
	static bool toRenderCosmos = true;

	Vec2 windowSize = getSDLWindowSize();

	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize(windowSize);
	ImGui::Begin("Cosmos", &toRenderCosmos, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | 
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);

	if(ImGui::IsWindowFocused()) {
		// Handle scroll wheel
		{
			Vec2 anchor = ImGui::GetMousePos();
			if(wheelEvent > 0) {
				setZoom(std::min(1.0, cameraZoom * 1.25), anchor);
			} else if(wheelEvent < 0) {
				setZoom(cameraZoom / 1.25, anchor);
			}
		}

		// Handle mouse movement
		{
			if(ImGui::IsMouseDragging(0)) {
				Vec2 delta = ImGui::GetMouseDragDelta();
				cameraPosition.x -= delta.x / cameraZoom;
				cameraPosition.y -= delta.y / cameraZoom;
				ImGui::ResetMouseDragDelta();
			}
		}
	}
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Draw entities
	{
		Vec2 windowCenter = { windowSize.x / 2, windowSize.y / 2 };

		for(auto iter = logicState.getEntitiesBegin(); iter != logicState.getEntitiesEnd(); iter++) {
			Entity* entity = iter->get();
			PhysicalProperties* physicalProps = entity->getPhysicalProperties();
			Vec2* position = entity->getPosition();

			if(!physicalProps || !position)
				continue;

			float radius = static_cast<float>(std::max(physicalProps->minRadius, physicalProps->radius * cameraZoom));
			if(radius <= 0)
				continue;
			
			Vec2 drawPos = {
				windowCenter.x + (entity->position->x - cameraPosition.x) * cameraZoom,
				windowCenter.y + (entity->position->y - cameraPosition.y) * cameraZoom
			};

			drawList->AddCircleFilled(drawPos, radius, ImColor(entity->getColor()), 100);
		}
	}
	ImGui::End();
}