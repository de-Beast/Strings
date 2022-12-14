#pragma once
#include <memory>
#include <SDL_ttf.h>
#include <string>

class FontManager
{
public:
	FontManager() = delete;
	~FontManager() = delete;

	static std::shared_ptr<TTF_Font> get_font(const int font_size = DEFAULT_FONT_SIZE);

	inline static constexpr int DEFAULT_FONT_SIZE = 40;

private:
	inline static std::string _font_path = "font/CascadiaCode.ttf";
};
