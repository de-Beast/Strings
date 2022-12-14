#include "TextBox.h"

#include <format>

#include "../Globals.h"

TextBox::TextBox(const ID& id, SDL_Renderer* renderer, const Dimensions& dim, const SDL_Point& coords, const std::string& text, const int font_size, const SDL_Color& background_color, const SDL_Color& foreground_color, const bool bResize_to_text, const bool is_visible)
	: UIElement(id)
{
	_is_visible = is_visible;

	SDL_Surface* text_surface = TTF_RenderUTF8_Blended_Wrapped(FontManager::get_font(font_size).get(), text.c_str(), foreground_color, dim.w);
	if (text_surface == nullptr)
	{
		SDL_LogError(2, "Text for object {%s} was not created: %s", Id.c_str(), SDL_GetError());
		return;
	}

	_width = dim.w > text_surface->w and bResize_to_text ? text_surface->w : dim.w;
	_height = dim.h < text_surface->h ? text_surface->h : dim.h;

	SDL_Surface* surface = create_backgroung_surface(background_color);
	if (surface == nullptr)
	{
		SDL_FreeSurface(text_surface);
		return;
	}

	SDL_Rect rect((_width - text_surface->w) / 2,
				(_height - text_surface->h) / 2,
				_width, _height);
	if (SDL_BlitSurface(text_surface, nullptr, surface, &rect) != 0)
	{
		SDL_LogError(2, "Text for object {%s} was not rendered: %s", id.c_str(), SDL_GetError());
		SDL_FreeSurface(text_surface);
		SDL_FreeSurface(surface);
		return;
	}

	_texture = create_texture(renderer, surface);
	if (_texture == nullptr)
	{
		SDL_FreeSurface(text_surface);
		SDL_FreeSurface(surface);
		return;
	}

	if(not move(coords))
		SDL_LogWarn(1, "Can not place object {%s} on screen", id.c_str());

	SDL_FreeSurface(surface);
	SDL_FreeSurface(text_surface);
}