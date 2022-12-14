#include "Application.h"

#include <iostream>
#include <SDL_ttf.h>

#include "Globals.h"
#include "page/MainMenuPage.h"

using std::cout, std::endl;

Application::Application()
{
	if (not init())
		return;

	_window = SDL_CreateWindow("Строки",
							   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   WINDOW_WIDTH, WINDOW_HEIGHT,
							   SDL_WINDOW_SHOWN);
	if (_window == nullptr)
	{
		SDL_LogCritical(0, "Window was not created: %s", SDL_GetError());
		return;
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr)
	{
		SDL_LogCritical(0, "Renderer was not created: %s", SDL_GetError());
		return;
	}

	Page::init_page<MainMenuPage>(_renderer)->draw();

	loop();
}

Application::~Application()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	TTF_Quit();
	SDL_Quit();
}

void Application::start_application()
{
	if (_app == nullptr)
		_app = new Application();
}

bool Application::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_LogCritical(0, "SDL_Init Error: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() != 0)
	{
		SDL_LogCritical(0, "TTF_Init Error: %s", TTF_GetError());
		return false;
	}

	if (std::string msg; not Globals::init(msg))
	{
		SDL_LogCritical(0, "Init Error: %s", msg.c_str());
		return false;
	}

	return true;
}

void Application::handle_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
		switch (event.type)
		{
			case SDL_QUIT:
			{
				_quit = true; break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				Page::get_page()->check_clicks({event.button.x, event.button.y});
				break;
			}

			case SDL_KEYDOWN:
			{
				if (SDL_IsTextInputActive() and event.key.keysym.sym == SDLK_BACKSPACE)
				{
					case SDL_TEXTINPUT:
						Page::get_page()->process_text_input(event);
				}
				break;
			}
		}
}

void Application::loop()
{
	while (not _quit)
	{
		handle_events();
		Page::get_page()->draw();
		SDL_RenderPresent(_renderer);
	}
}
