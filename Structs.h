#pragma once

#include <memory>
#include <SDL_pixels.h>

struct Colors
{
	static constexpr SDL_Color RED = {255, 0, 0, 255};
	static constexpr SDL_Color BLUE = {0, 0, 255, 255};
	static constexpr SDL_Color GREEN = {0, 255, 0, 255};
	static constexpr SDL_Color WHITE = {255, 255, 255, 255};
	static constexpr SDL_Color BLACK = {0, 0, 0, 255};
};

struct Dimensions
{
	int w, h;
};

struct EventArgs
{
};
using EventArguments = std::shared_ptr<EventArgs>;
