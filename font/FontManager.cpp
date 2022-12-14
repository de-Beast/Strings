#include "FontManager.h"

std::shared_ptr<TTF_Font> FontManager::get_font(const int font_size)
{
	TTF_Font* font = TTF_OpenFont(_font_path.c_str(), font_size);
	return {font, &TTF_CloseFont};
}
