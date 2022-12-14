#pragma once
#include "Page.h"

class InfoPage : public Page
{
public:
	explicit InfoPage(SDL_Renderer* const renderer);

private:
	static void back_button(const EventArguments&);
	
	void init_bind_buttons() override;
};