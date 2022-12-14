#pragma once

#include <string>
#include <set>

#include "SDL_ttf.h"
#include "../Structs.h"

using ID = std::string;

class UIElement
{
public:
	UIElement(const ID& id, SDL_Renderer* renderer, const Dimensions& dim, const SDL_Point& coords, const SDL_Color& background_color, const bool is_visible = true);
	virtual ~UIElement();

	__declspec(property(get = get_id)) const ID& Id;
	const ID& get_id() const;
	__declspec(property(get = is_visible, put = set_visibility)) bool bVisible;
	bool is_visible() const;
	void set_visibility(const bool is_visible = true);

	__declspec(property(get = get_x)) const int& X;
	virtual const int& get_x() const;
	__declspec(property(get = get_y)) const int& Y;
	virtual const int& get_y() const;

	__declspec(property(get = get_width)) const int& W;
	virtual const int& get_width() const;
	__declspec(property(get = get_height)) const int& H;
	virtual const int& get_height() const;

	void unregister();
	bool move(const SDL_Point& coords);
	virtual void draw(SDL_Renderer* renderer) const;

protected:
	UIElement(const ID& id, bool check_id = true);

	SDL_Surface* create_backgroung_surface(const SDL_Color& bckg_color) const;
	SDL_Texture* create_texture(SDL_Renderer* renderer, SDL_Surface* surface) const;

	int _width = 0, _height = 0;
	SDL_Texture* _texture = nullptr;
	bool _is_visible = true;

private:
	UIElement(const ID& id, const Dimensions& dim, bool check_id = true);

	inline static std::set<ID> _ids;

	ID _id;
	int _x = -1, _y = -1;
};
