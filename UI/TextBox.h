#pragma once

#include "UIElement.h"

#include "../font/FontManager.h"

class TextBox : public UIElement
{
public:
	TextBox(const ID& id, SDL_Renderer* renderer, const Dimensions& dim, const SDL_Point& coords, const std::string& text, const int font_size = FontManager::DEFAULT_FONT_SIZE, const SDL_Color& background_color = Colors::BLACK, const SDL_Color& foreground_color = Colors::WHITE, const bool bResize_to_text = false, bool is_visible = true);
};
