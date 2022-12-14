#pragma once

#include <SDL.h>

#include "Page.h"

class MainMenuPage : public Page
{
public:
	explicit MainMenuPage(SDL_Renderer* renderer);

private:
	static void start_button(const EventArguments&);
	static void info_button(const EventArguments&);
	static void quit_button(const EventArguments&);

	void init_bind_buttons() override;
};
