#include "Page.h"

#include <ranges>

Page::Page(SDL_Renderer* renderer)
	:_renderer(renderer){}

void Page::draw() const
{
	SDL_RenderFillRect(_renderer, nullptr);
	for (const auto& text_block : _ui_elems | std::views::values)
		text_block->draw(_renderer);
}

void Page::clear()
{
	for (const auto& elem : _ui_elems | std::views::values)
		elem->unregister();
	_ui_elems.clear();
}

void Page::check_clicks(const SDL_Point& mouse_click_coords)
{
	InputBox::reset_focus();
	std::shared_ptr<IClickable> found_clickable;
	for (auto& box : _ui_elems | std::views::values)
	{
		std::shared_ptr<IClickable> clickable = std::dynamic_pointer_cast<IClickable>(box);
		if (clickable != nullptr and clickable->is_clicked(mouse_click_coords))
			found_clickable = clickable;
	}

	if (found_clickable != nullptr)
		found_clickable->on_click();
}

void Page::process_text_input(const SDL_Event& event)
{
	for (auto& box : _ui_elems | std::views::values)
	{
		if (std::shared_ptr<InputBox> input_box = std::dynamic_pointer_cast<InputBox>(box);
			input_box != nullptr and input_box->is_focused())
		{
			input_box->process_text_input(event, _renderer);
			break;
		}
	}
}
