#include "ui_manager.h"

#include <algorithm>
#include <iostream>
#include <ctime>

#include "imgui.h"
#include "logic_manager.h"

#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe'
#endif

Vec2 UIManager::getSDLWindowSize() const {
	int width;
	int height;
	SDL_GetWindowSize(SDLwindow, &width, &height);
	return { static_cast<double>(width), static_cast<double>(height) };
}

void UIManager::render(LogicManager& logicState, int wheelEvent) {
	renderCosmos(logicState, wheelEvent);
	
	if(infoOpen) renderInfo(logicState);
	if(logOpen) renderLog();

	if(logicState.universeAdvancing) {
		universeTime timeStep = ImGui::GetIO().DeltaTime * static_cast<double>(logicState.timeScale);
		logicState.clockForward(timeStep);
	}
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

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize(windowSize);
	ImGui::Begin("Cosmos", &toRenderCosmos, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar);

	if(ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("Menus")) {
			ImGui::MenuItem("Info", NULL, &infoOpen);
			ImGui::MenuItem("Log", NULL, &logOpen);
			ImGui::EndMenu();
		}
		ImGui::Text(" |   FPS: %.0f (%.2f ms)", ImGui::GetIO().Framerate, ImGui::GetIO().DeltaTime * 1000.0f);
		//ImGui::MenuItem(" | Shit", NULL, nullptr, false);
		ImGui::EndMenuBar();
	}

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
	Vec2 windowCenter = { windowSize.x / 2, windowSize.y / 2 };

	Vec2& camera = cameraPosition;
	double& zoom = cameraZoom;

	// Draw orbits
	logicState.forEntities( [&drawList, &windowCenter, &camera, &zoom](Entity* entity) {
		KeplerOrbit* orbitalProps = entity->getOrbitalProperties();
		Vec2* position = entity->getPosition();
		Entity* parentEntity = entity->getParentEntity();

		if(!orbitalProps || !position || !parentEntity)
			return;

		Vec2 orbitCenter = orbitalProps->getCenter(*parentEntity->getPosition());
		Vec2 drawCenter = {
			windowCenter.x + (orbitCenter.x - camera.x) * zoom,
			windowCenter.y + (orbitCenter.y - camera.y) * zoom
		};

		float orbitAlpha = 1.0f - static_cast<float>(orbitalProps->getSemimajorAxis() * zoom / Util::MAX_ELLIPSE_SEMIMAJOR);
		if(orbitAlpha >= 0.99f)
			orbitAlpha = 1.0f;

		if(orbitAlpha > 0.0f) {
			drawList->AddEllipse(drawCenter, static_cast<float>(orbitalProps->getSemimajorAxis() * zoom),
				static_cast<float>(orbitalProps->getSemiminorAxis() * zoom),
				ImColor(Vec4(113.0f / 255.0f, 117.0f / 255.0f, 130.0f / 255.0f, orbitAlpha)), static_cast<float>(-orbitalProps->getLPeriapsis()), 100);
		}
	});

	// Draw entities
	logicState.forEntities([&drawList, &windowCenter, &camera, &zoom](Entity* entity) {
		PhysicalProperties* physicalProps = entity->getPhysicalProperties();
		Vec2* position = entity->getPosition();

		if(!physicalProps || !position)
			return;

		float radius = static_cast<float>(std::max(physicalProps->minRadius, physicalProps->radius * zoom));
		if(radius <= 0)
			return;

		Vec2 drawPos = {
			windowCenter.x + (entity->position->x - camera.x) * zoom,
			windowCenter.y + (entity->position->y - camera.y) * zoom
		};

		drawList->AddCircleFilled(drawPos, radius, ImColor(entity->getColor()), 100);
		if(radius > 2 || zoom >= 0.0001 || entity->getType() == EntityType::star) {
			std::string name = entity->getName();
			drawList->AddText(Vec2(drawPos.x - name.size() * 3.5, drawPos.y + radius + 2), ImColor(entity->getColor()), name.data());
		}
	});
	ImGui::End();
}

void UIManager::renderInfo(LogicManager& logicState) {
	if(!ImGui::Begin("Info", &infoOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}
	ImGui::Text("Camera: (%.3f, %.3f) km", cameraPosition.x, cameraPosition.y);
	ImGui::Text("Zoom: x%.10f", cameraZoom);
	time_t currentTime = logicState.getUniverseTime();
	ImGui::Text("Time: %s", std::ctime(&currentTime));
	ImGui::TextColored(Vec4(0.55, 0.55, 0.55, 1.0), "(%f s since J2000)", logicState.getUniverseClock());
	ImGui::Checkbox("Realtime system simulation", &logicState.universeAdvancing);
	ImGui::SliderInt("Time multiplier", &logicState.timeScale, 1, 10000000);
	ImGui::End();
}

void UIManager::renderLog() {
	ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_FirstUseEver);
	if(!ImGui::Begin("Log", &logOpen)) {
		ImGui::End();
		return;
	}
	ImGui::BeginChild("LoggerRegion", Vec2(0.0, -1), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	for(const std::string& msg : loggedMessages) {
		ImGui::TextUnformatted(msg.data());
	}
	ImGui::EndChild();
	ImGui::End();
}

void UIManager::log(const char* msg) {
	loggedMessages.emplace_back(msg);
	if(logOpen) {
		ImGui::Begin("Log");
		ImGui::BeginChild("LoggerRegion");
		ImGui::SetScrollY(ImGui::GetScrollMaxY());
		ImGui::EndChild();
		ImGui::End();
	}
}