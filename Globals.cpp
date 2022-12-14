#include "Globals.h"

#include "SDL.h"

bool Globals::init(std::string& message)
{
	SDL_DisplayMode display_mode;
	if (SDL_GetDesktopDisplayMode(0, &display_mode) < 0)
	{
		message = "SDL_GetDesktopDisplayMode: Can not get display mode";
		return false;
	}
	_WINDOW_WIDTH = display_mode.w / 2;
	_WINDOW_HEIGHT = display_mode.h / 2;

	return true;
}

int Globals::get_window_width()
{
	return _WINDOW_WIDTH;
}

int Globals::get_window_height()
{
	return _WINDOW_HEIGHT;
}
