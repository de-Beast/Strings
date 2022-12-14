#include "Application.h"

#undef main

int SDL_main(int argc, char *argv[])
{
	Application::start_application();

	return 0;
}