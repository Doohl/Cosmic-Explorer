#pragma once

#include <SDL.h>
#include <vector>

#include "imgui.h"
#include "vec2.h"

class LogicManager;


class UIManager {
public:
	UIManager(SDL_Window* window)
		: SDLwindow(window)
		{}

	// Render the entire game UI
	void render(LogicManager& logicState, int wheelEvent);

	// Get the size of the active SDL window
	Vec2 getSDLWindowSize() const;

	// Set a new zoom factor
	void setZoom(double newZoom);
	void setZoom(double newZoom, Vec2 anchor);

	void log(const char* msg);

	bool infoOpen = true;
	bool logOpen = false;
	bool browserOpen = false;

private:
	// Render game entities on the 'cosmos' widget
	void renderCosmos(LogicManager& logicState, int wheelEvent);

	void renderInfo(LogicManager& logicState);
	void renderLog();
	void renderBrowser(LogicManager& logicState);

	// The current coordinates of the client camera
	Vec2 cameraPosition = { 0, 0 };
	// The zoom factor of the client camera
	double cameraZoom = 0.000002f;

	Vec2 prevMousePos = { -1, -1 };

	// Simple pointer to the active SDL window
	SDL_Window* SDLwindow;

	std::vector<std::string> loggedMessages;
};