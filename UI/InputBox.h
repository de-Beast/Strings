#pragma once

#include "UIElement.h"
#include "TextBox.h"
#include "../Interfaces.h"

class InputBox : public UIElement, public IClickable
{
public:

	InputBox(const ID& id, SDL_Renderer* const renderer, const Dimensions& dim, const SDL_Point& coords, int font_size, const SDL_Color& background_color);

	static void reset_focus();

	__declspec(property(get = get_text)) const std::string& Text;
	const std::string& get_text() const;
	bool set_text(SDL_Renderer* renderer, const std::string& text = "");

	void set_enable(const bool is_enabled) override;

	bool is_focused() const;

	void set_message(SDL_Renderer* renderer = nullptr, const std::string& message = "", const bool is_error_message = true, const int font_size = 0, const SDL_Color& font_color = Colors::RED, const Dimensions& dims = Dimensions(0, 0));
	void process_text_input(const SDL_Event& event, SDL_Renderer* renderer);

	bool is_clicked(const SDL_Point& mouse_click_coords) override;
	void on_click() override;

	void draw(SDL_Renderer* renderer) const override;

private:
	void update_texture(SDL_Renderer* renderer, const std::string& new_string);
	void draw_underline(SDL_Renderer* renderer, const SDL_Color& line_color) const;

	inline static bool _focused_is_found = false;

	std::string _text;
	std::shared_ptr<TextBox> _message_box = nullptr;
	bool _is_error_message = false;
	bool _bIs_focused = false;
	int _font_size;
	SDL_Color _background_color;
};
