#pragma once
#include <SDL.h>

class IClickable
{
public:
	IClickable() = default;
	virtual ~IClickable() = default;

	__declspec(property(get = is_enabled, put = set_enable)) bool bEnabled;
	bool is_enabled() const
	{
		return _is_enabled;
	}
	virtual void set_enable(const bool is_enabled)
	{
		_is_enabled = is_enabled;
	}
	virtual bool is_clicked(const SDL_Point& mouse_click_coords) = 0;
	virtual void on_click() = 0;

private:
	bool _is_enabled = true;
};
