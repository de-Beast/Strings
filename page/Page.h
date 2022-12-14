#pragma once

#include <map>
#include <memory>
#include <stdexcept>

#include <SDL.h>

#include "../Structs.h"
#include "../UI.h"


class Page 
{
public:
	Page() = delete;
	Page(const Page&) = delete;
	Page& operator=(const Page&) = delete;
	Page(const Page&&) = delete;
	Page& operator=(const Page&&) = delete;

	virtual ~Page() = default;

	template<class T> requires std::derived_from<T, Page>
	static std::shared_ptr<T> init_page(SDL_Renderer* renderer);
	template<class T = Page> requires std::derived_from<T, Page>
	static std::shared_ptr<T> get_page();

	virtual void check_clicks(const SDL_Point& mouse_click_coords);
	virtual void process_text_input(const SDL_Event& event);
	virtual void draw() const;
	void clear();

protected:
	struct ButtonArgs : EventArgs
	{
		std::weak_ptr<Button> button;

		ButtonArgs(const std::shared_ptr<Button>& button)
			: button(button){}
	};

	struct InputBoxAndButtonArgs : ButtonArgs
	{
		std::weak_ptr<InputBox> input_box;

		InputBoxAndButtonArgs(const std::shared_ptr<Button>& button, const std::shared_ptr<InputBox>& input_box)
			: ButtonArgs(button), input_box(input_box){}
	};

	explicit Page(SDL_Renderer* renderer);

	template<class T>
	static std::shared_ptr<T> get_object(const ID& id);

	template<class T> requires std::derived_from<T, TextBox>
	std::shared_ptr<T> create_object(const ID& id, const Dimensions& dimens, const SDL_Point& coords, const std::string& text, const int font_size = FontManager::DEFAULT_FONT_SIZE, const SDL_Color& background_color = Colors::BLACK, const SDL_Color& foreground_color = Colors::WHITE, const bool do_resize_to_text = false);
	template<class T> requires std::derived_from<T, TextBox>
	std::shared_ptr<T> create_object(const ID& id, const Dimensions& dimens, const SDL_Point& coords, const std::string& text, const int font_size, const bool do_resize_to_text);
	template<class T> requires std::derived_from<T, InputBox>
	std::shared_ptr<T> create_object(const ID& id, const Dimensions& dimens, const SDL_Point& coords, const int font_size = FontManager::DEFAULT_FONT_SIZE, const SDL_Color& background_color = Colors::WHITE);

	SDL_Renderer* const _renderer;
	std::map<ID, std::shared_ptr<UIElement>> _ui_elems;

private:
	inline static std::shared_ptr<Page> _instance = std::shared_ptr<Page>(nullptr);

	virtual void init_bind_buttons() = 0;
};

template<class T> requires std::derived_from<T, Page>
std::shared_ptr<T> Page::init_page(SDL_Renderer* renderer)
{
	if (_instance != nullptr)
		_instance->clear();

	_instance = std::make_shared<T>(renderer);
	return std::dynamic_pointer_cast<T>(_instance);
}

template <class T> requires std::derived_from<T, Page>
std::shared_ptr<T> Page::get_page()
{
	if (_instance == nullptr)
		throw std::runtime_error("Page is not initialized");

	return std::dynamic_pointer_cast<T>(_instance);
}

template <class T>
std::shared_ptr<T> Page::get_object(const ID& id)
{
	return std::dynamic_pointer_cast<T>(_instance->_ui_elems.at(id));
}

template <class T> requires std::derived_from<T, TextBox>
std::shared_ptr<T> Page::create_object(const ID& id, const Dimensions& dimens, const SDL_Point& coords, const std::string& text, const int font_size, const bool do_resize_to_text)
{
	auto obj = std::make_shared<T>(id,
								   _renderer,
								   dimens,
								   coords,
								   text,
								   font_size,
								   Colors::BLACK,
								   Colors::WHITE,
								   do_resize_to_text);
	_ui_elems.insert({obj->Id, obj});
	return obj;
}

template <class T> requires std::derived_from<T, TextBox>
std::shared_ptr<T> Page::create_object(const ID& id, const Dimensions& dimens, const SDL_Point& coords, const std::string& text, const int font_size, const SDL_Color& background_color, const SDL_Color& foreground_color, const bool do_resize_to_text)
{
	auto obj = std::make_shared<T>(id,
								   _renderer,
								   dimens,
								   coords,
								   text,
								   font_size,
								   background_color,
								   foreground_color,
								   do_resize_to_text);
	_ui_elems.insert({obj->Id, obj});
	return obj;
}

template<class T> requires std::derived_from<T, InputBox>
std::shared_ptr<T> Page::create_object(const ID& id, const Dimensions& dimens, const SDL_Point& coords, const int font_size, const SDL_Color& background_color)
{
	auto obj = std::make_shared<T>(id,
									_renderer,
									dimens,
									coords,
									font_size,
									background_color);
	_ui_elems.insert({obj->Id, obj});
	return obj;
}
