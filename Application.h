#pragma once

#include <SDL.h>
#include <vector>

#include "page/Page.h"
#include "string/BaseString.h"

class Application
{
public:
	~Application();

	static void start_application();

private:
	Application();
	inline static Application* _app = nullptr;

	static bool init();

	void loop();
	void handle_events();

	std::vector<BaseString*> _strings;
	using cur_type = Page;
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	bool _quit = false;
};