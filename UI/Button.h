#pragma once

#include <functional>

#include "../Structs.h"
#include "../Interfaces.h"

#include "TextBox.h"


using ButtonCallback = std::function<void (const EventArguments&)>;

class Button : public TextBox, public IClickable
{
public:
	Button(const ID& id, SDL_Renderer* const renderer, const Dimensions& dim, const SDL_Point& coords, const std::string& text, const int font_size = FontManager::DEFAULT_FONT_SIZE, const SDL_Color& background_color = Colors::BLACK, const SDL_Color& foreground_color = Colors::WHITE, const bool do_resize_to_text = false, const bool is_visible = true)
		: TextBox(id, renderer, dim, coords, text, font_size, background_color, foreground_color, do_resize_to_text, is_visible){}

	~Button() override;
	EventArguments get_args() const;
	void set_enable(const bool is_enabled) override;
	
	void bind_on_click(const ButtonCallback& callback, const EventArguments& args = nullptr);
	void bind_on_click(const EventArguments& args);
	void unbind();
	bool is_clicked(const SDL_Point& mouse_click_coords) override;
	void on_click() override;

private:
	ButtonCallback _callback = ButtonCallback(nullptr);
	EventArguments _args = EventArguments(nullptr);
};