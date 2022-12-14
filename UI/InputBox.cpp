#include "InputBox.h"

#include <iostream>

#include "../Globals.h"
#include "../Structs.h"

InputBox::InputBox(const ID& id, SDL_Renderer* const renderer, const Dimensions& dim, const SDL_Point& coords, int font_size, const SDL_Color& background_color)
	: UIElement(id, renderer, dim, coords, background_color), _font_size(font_size), _background_color(background_color){}

void InputBox::reset_focus()
{
	_focused_is_found = false;
}

const std::string& InputBox::get_text() const
{
	return _text;
}

bool InputBox::set_text(SDL_Renderer* renderer, const std::string& text)
{
	update_texture(renderer, text);
	return _text == text;
}

void InputBox::set_enable(const bool is_enabled)
{
	IClickable::set_enable(is_enabled);
	if (bEnabled)
		SDL_SetTextureColorMod(_texture, 255, 255, 255);
	else
		SDL_SetTextureColorMod(_texture, 100, 100, 100);
}

bool InputBox::is_focused() const
{
	return _bIs_focused;
}

void InputBox::set_message(SDL_Renderer* renderer, const std::string& message, const bool is_error_message, const int font_size, const SDL_Color& font_color, const Dimensions& dims)
{
	_message_box = nullptr;
	_is_error_message = false;
	if (not message.empty())
	{
		_is_error_message = is_error_message;
		_message_box = std::make_shared<TextBox>(Id + "LimitMessage", renderer, dims.w == 0 or dims.h == 0 ? Dimensions(W, H) : dims, SDL_Point(X, Y + H), message, font_size > 0 ? font_size : _font_size, Colors::BLACK, font_color);
	}
}

void InputBox::process_text_input(const SDL_Event& event, SDL_Renderer* renderer)
{
	std::string new_string = _text;
	switch (event.type)
	{
		case SDL_TEXTINPUT:
		{
			const char new_char = event.text.text[0];
			if (new_char >= 0 and (new_char >= 'A' and new_char <= 'Z' or new_char >= 'a' and new_char <= 'z' or std::isdigit(new_char) or std::ispunct(new_char)))
			{
				new_string.push_back(new_char);
				break;
			}
			return;
		}
		case SDL_KEYDOWN:
		{
			if (not new_string.empty())
			{
				new_string.pop_back();
				break;
			}
			return;
		}
	}

	update_texture(renderer, new_string);
}

bool InputBox::is_clicked(const SDL_Point& mouse_click_coords)
{
	if (not bEnabled or not _is_visible)
		return false;
	
	bool cond = mouse_click_coords.x >= X and mouse_click_coords.x <= X + W;
	cond &= mouse_click_coords.y >= Y and mouse_click_coords.y <= Y + H;
	_bIs_focused = cond;

	if (_bIs_focused)
		_focused_is_found = true;
	if (SDL_IsTextInputActive() and not _focused_is_found)
		SDL_StopTextInput();

	return _bIs_focused;
}

void InputBox::on_click()
{
	if (not SDL_IsTextInputActive() and _bIs_focused)
		SDL_StartTextInput();
}

void InputBox::draw(SDL_Renderer* renderer) const
{
	UIElement::draw(renderer);
	if (_message_box)
	{
		_message_box->draw(renderer);
	}
	if (_bIs_focused)
	{
		_is_error_message ? draw_underline(renderer, Colors::RED) : draw_underline(renderer, Colors::BLUE);
	}
}

void InputBox::update_texture(SDL_Renderer* renderer, const std::string& new_string)
{
	SDL_Surface* background_surface = create_backgroung_surface(_background_color);
	if (background_surface == nullptr)
	{
		std::cout << "Error: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_Surface* text_surface = nullptr;
	if (not new_string.empty())
	{
		text_surface = TTF_RenderUTF8_Blended(FontManager::get_font(_font_size).get(), new_string.c_str(), SDL_Color(255 - _background_color.r, 255 - _background_color.g, 255 - _background_color.b));
		if (text_surface == nullptr)
		{
			std::cout << "Error: " << SDL_GetError() << std::endl;
			SDL_FreeSurface(background_surface);
			return;
		}

		if (_width < text_surface->w)
		{
			SDL_FreeSurface(text_surface);
			text_surface = TTF_RenderUTF8_Blended(FontManager::get_font(_font_size).get(), _text.c_str(), SDL_Color(255 - _background_color.r, 255 - _background_color.g, 255 - _background_color.b));
		}
		else
		{
			_text = new_string;
		}

		SDL_Rect text_rect(0, 0, _width, _height);
		if (SDL_BlitSurface(text_surface, nullptr, background_surface, &text_rect) != 0)
		{
			SDL_LogError(2, "Text for object {%s} was not rendered: %s", Id.c_str(), SDL_GetError());
			SDL_FreeSurface(text_surface);
			SDL_FreeSurface(background_surface);
			return;
		}
	}
	else
	{
		_text = new_string;
	}

	SDL_DestroyTexture(_texture);
	_texture = create_texture(renderer, background_surface);
	if (_texture == nullptr)
	{
		SDL_LogError(2, "Texture for object {%s} was not created: %s", Id.c_str(), SDL_GetError());
		SDL_FreeSurface(background_surface);
		SDL_FreeSurface(text_surface);
		return;
	}
	set_enable(bEnabled);

	SDL_FreeSurface(background_surface);
	SDL_FreeSurface(text_surface);
}

void InputBox::draw_underline(SDL_Renderer* renderer, const SDL_Color& line_color) const 
{
	SDL_SetRenderDrawColor(renderer, line_color.r, line_color.g, line_color.b, SDL_ALPHA_OPAQUE);
	for (int i = 0; i < 2; i++)
		SDL_RenderDrawLine(renderer, X, Y + H + i, X + W-1, Y + H + i);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}
