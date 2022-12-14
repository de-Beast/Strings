#include "UIElement.h"

#include <format>
#include <ranges>

#include "../Globals.h"

UIElement::UIElement(const ID& id, SDL_Renderer* renderer, const Dimensions& dim, const SDL_Point& coords, const SDL_Color& background_color, const bool is_visible)
	: UIElement(id, dim)
{
	_is_visible = is_visible;
	SDL_Surface* surface = create_backgroung_surface(background_color);
	if (surface == nullptr)
		return;

	_texture = create_texture(renderer, surface);
	if (_texture == nullptr)
	{
		SDL_FreeSurface(surface);
		return;
	}

	if (not move(coords))
		SDL_LogWarn(1, "Can not place object {%s} on screen", id.c_str());

	SDL_FreeSurface(surface);
}

UIElement::UIElement(const ID& id, const Dimensions& dim, bool check_id)
	: UIElement(id, check_id)
{
	_width = dim.w;
	_height = dim.h;
}

UIElement::UIElement(const ID& id, bool check_id)
	: _id(id)
{
	if (check_id and _ids.contains(id))
	{
		throw std::runtime_error(std::format("Object with given id({}) already exists", id));
	}
	_ids.insert(id);
}

UIElement::~UIElement()
{
	_ids.erase(_id);
	SDL_DestroyTexture(_texture);
}


const ID& UIElement::get_id() const
{
	return _id;
}

bool UIElement::is_visible() const
{
	return _is_visible;
}

void UIElement::set_visibility(const bool is_visible)
{
	_is_visible = is_visible;
}

const int& UIElement::get_x() const
{
	return _x;
}

const int& UIElement::get_y() const
{
	return _y;
}

const int& UIElement::get_width() const
{
	return _width;
}

const int& UIElement::get_height() const
{
	return _height;
}

void UIElement::unregister()
{
	_ids.erase(_id);
	_id = "";
}

bool UIElement::move(const SDL_Point& coords)
{
	bool result = false;
	if (coords.x >= 0 and coords.x <= WINDOW_WIDTH)
	{
		_x = coords.x;
		result = true;
	}
	if (coords.y >= 0 and coords.y <= WINDOW_HEIGHT)
	{
		_y = coords.y;
		result = true;
	}
	return result;
}

void UIElement::draw(SDL_Renderer* renderer) const
{
	if (not _is_visible)
		return;

	const SDL_Rect rect(_x, _y, _width, _height);
	if (SDL_RenderCopy(renderer, _texture, nullptr, &rect) != 0)
		SDL_LogError(2, "Object {%s} was not drawn: %s", _id.c_str(), SDL_GetError());
}


SDL_Surface* UIElement::create_backgroung_surface(const SDL_Color& bckg_color) const
{
	SDL_Surface* surface = SDL_CreateRGBSurface(0, _width, _height, 32, 0, 0, 0, 0);
	if (surface == nullptr)
	{
		SDL_LogError(2, "Surface for object {%s} was not created: %s", _id.c_str(), SDL_GetError());
		return nullptr;
	}

	if (SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, bckg_color.r, bckg_color.g, bckg_color.b)) != 0)
	{
		SDL_LogError(2, "Object {%s} was not colored: %s", _id.c_str(), SDL_GetError());
		SDL_FreeSurface(surface);
		return nullptr;
	}
	return surface;
}


SDL_Texture* UIElement::create_texture(SDL_Renderer* renderer, SDL_Surface* surface) const
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr)
	{
		SDL_LogError(2, "Texture for object {%s} was not created: %s", _id.c_str(), SDL_GetError());
		return nullptr;
	}
	return texture;
}
