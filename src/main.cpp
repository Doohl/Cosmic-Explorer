#include <stdio.h>
#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>

#if defined __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL_opengles2.h>
#else
#include "gl3w.h"
#endif

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "logic_manager.h"
#include "ui_manager.h"

#if defined __EMSCRIPTEN__
#include <functional>
static std::function<void()> loop;
static void main_loop() { loop(); }
int main(int, char**) {
#elif defined _WIN32 || defined _WIN64
int SDL_main(int, char**) {
#else
int main(int, char**) {
#endif
	// Setup SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		std::cerr << "Error: \n" << SDL_GetError();
		return -1;
	}

#if defined __EMSCRIPTEN__
	const char* glsl_version = "#version 300 es";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Cosmic Explorer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // Enable vsync

#ifndef __EMSCRIPTEN__
	if(gl3wInit() != 0) {
		std::cerr << "Failed to initialize OpenGL loader!\n";
		return 1;
	}
#endif

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// bool show_demo_window = true;
    // bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.05f, 0.10f, 0.25f, 1.00f);

	LogicManager logic(LogicManager::GetTimeSinceJ2000());
	UIManager uiManager(window);

	logic.initializeSol();

	bool done = false;
#if defined __EMSCRIPTEN__
	loop = [&]() {
#else
	while(!done) {
#endif
		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if(event.type == SDL_QUIT)
				done = true;
			if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				done = true;
			if(event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
				uiManager.onScroll(1.25);
			if(event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
				uiManager.onScroll(-1.25);
			//if(event.type == SDL_MULTIGESTURE) {
			//	if(std::fabs(event.mgesture.dDist) > 0.002) {
			//		Vec2 windowSize = uiManager.getSDLWindowSize();
			//		float touchX = windowSize.x * event.mgesture.x;
			//		float touchY = windowSize.y * event.mgesture.y;
			//	}
			//}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		uiManager.render(logic);

		// if(show_demo_window)
			// ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Render();
		SDL_GL_MakeCurrent(window, gl_context);
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	};
#if defined __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop, 0, true);
#endif
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}