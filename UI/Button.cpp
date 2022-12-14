#include "Button.h"

Button::~Button()
{
	unbind();
}

EventArguments Button::get_args() const
{
	return _args;
}

void Button::set_enable(const bool is_enabled)
{
	IClickable::set_enable(is_enabled);
	if (bEnabled)
		SDL_SetTextureColorMod(_texture, 255, 255, 255);
	else
		SDL_SetTextureColorMod(_texture, 100, 100, 100);
}

void Button::bind_on_click(const ButtonCallback& callback, const EventArguments& args)
{
	_callback = callback;
	if (args != nullptr)
		_args = args;
}
void Button::bind_on_click(const EventArguments& args)
{
	_args = args;
}

void Button::unbind()
{
	_callback = ButtonCallback(nullptr);
	_args = EventArguments(nullptr);
}

bool Button::is_clicked(const SDL_Point& mouse_click_coords)
{
	if (not bEnabled or not _is_visible)
		return false;

	bool cond = mouse_click_coords.x >= X and mouse_click_coords.x <= X + W;
	cond &= mouse_click_coords.y >= Y and mouse_click_coords.y <= Y + H;
	return cond;
}

void Button::on_click()
{
	if (_callback != nullptr)
		_callback(_args);
}
