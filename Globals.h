#pragma once

#include <string>

#define WINDOW_WIDTH Globals::get_window_width()
#define WINDOW_HEIGHT Globals::get_window_height()

class Globals
{
public:
	static bool init(std::string& message);

	static int get_window_width();
	static int get_window_height();

private:
	inline static int _WINDOW_WIDTH = 0;
	inline static int _WINDOW_HEIGHT = 0;
};
