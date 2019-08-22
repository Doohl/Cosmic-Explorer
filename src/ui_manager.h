#pragma once

#include <SDL.h>

#include "imgui.h"
#include "vec2.h"

class LogicManager;


class UIManager {
public:
	UIManager(SDL_Window* window)
		: cameraPosition({ 0, 0 }),
		cameraZoom(0.000002f),
		prevMousePos({ -1, -1 }),
		SDLwindow(window)
		{}

	// Render the entire game UI
	void render(LogicManager& logicState, int wheelEvent);

	// Get the size of the active SDL window
	Vec2 getSDLWindowSize() const;

	// Set a new zoom factor
	void setZoom(double newZoom);
	void setZoom(double newZoom, Vec2 anchor);

private:
	// Render game entities on the 'cosmos' widget
	void renderCosmos(LogicManager& logicState, int wheelEvent);

	// The current coordinates of the client camera
	Vec2 cameraPosition;
	// The zoom factor of the client camera
	double cameraZoom;

	Vec2 prevMousePos;

	// Simple pointer to the active SDL window
	SDL_Window* SDLwindow;
};