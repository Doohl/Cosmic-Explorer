#pragma once

#include <SDL.h>

#include "imgui.h"
#include "vec2.h"

class LogicManager;


class UIManager {
public:
	UIManager(SDL_Window* window)
		: cameraPosition({0, 0}),
		SDLwindow(window)
		{}

	// Render the entire game UI
	void render(const LogicManager& logicState);

	// Get the size of the active SDL window
	ImVec2 getSDLWindowSize() const;

private:
	// Render game entities on the 'cosmos' widget
	void renderCosmos(const LogicManager& logicState) const;

	// The current coordinates of the client camera
	Vec2 cameraPosition;

	// Simple pointer to the active SDL window
	SDL_Window* SDLwindow;
};