#include "InitStringsPage.h"

#include <format>
#include <stdexcept>

#include "MainMenuPage.h"
#include "StringMenuPage.h"
#include "../string/StringDataBase.h"

#include "../Globals.h"
#include "../UI/Button.h"
#include "../UI/InputBox.h"


InitStringsPage::InitStringsPage(SDL_Renderer* const renderer)
	: Page(renderer)
{
	Dimensions dims{900, 50};
	create_object<TextBox>("TextBox1", dims, {(WINDOW_WIDTH - dims.w) / 2, WINDOW_HEIGHT / 3 - dims.h}, std::format("Введите колличество строк: {} максимум", _max_amount));

	dims = {500, 50};
	create_object<InputBox>("StringsAmountInput", dims, {(WINDOW_WIDTH - dims.w) / 2, 2 * WINDOW_HEIGHT / 3 - 3 * dims.h});

	dims = {120, 30};
	create_object<Button>("BackButton", dims, {30, WINDOW_HEIGHT - 2 * dims.h}, "Назад", 30);
	dims = {270, 30};
	create_object<Button>("ConfirmButton", dims, {WINDOW_WIDTH - dims.w - 30,  WINDOW_HEIGHT - 2 * dims.h}, "Подтвердить", 30);
	std::dynamic_pointer_cast<IClickable>(_ui_elems.at("ConfirmButton"))->bEnabled = false;

	InitStringsPage::init_bind_buttons();
}

void InitStringsPage::process_text_input(const SDL_Event& event)
{
	Page::process_text_input(event);
	check_input(std::dynamic_pointer_cast<InputBox>(_ui_elems.at("StringsAmountInput")));
}

void InitStringsPage::back_button(const EventArguments&)
{
	init_page<MainMenuPage>(get_page<InitStringsPage>()->_renderer);
}

void InitStringsPage::confirm_button(const EventArguments& args)
{
	StringDataBase::init_instance(std::static_pointer_cast<InitStringsPageArgs>(args)->amount);
	init_page<StringMenuPage>(get_page<InitStringsPage>()->_renderer);
}

void InitStringsPage::init_bind_buttons()
{
	std::shared_ptr<Button> button;

	if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("BackButton"))) != nullptr)
		button->bind_on_click(&back_button);

	if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("ConfirmButton"))) != nullptr)
		button->bind_on_click(&confirm_button, std::make_shared<InitStringsPageArgs>(_strings_amount));
}

void InitStringsPage::check_input(const std::shared_ptr<InputBox>& input_box)
{
	size_t pos = 0;
	int number;
	const std::string text = input_box->Text;
	const auto button = std::dynamic_pointer_cast<IClickable>(_ui_elems.at("ConfirmButton"));

	input_box->set_message();
	try
	{
		number = std::stoi(text, &pos);
		if (pos != text.size() or number < 1 or number > _max_amount)
		{
			button->bEnabled = false;
			if (pos != text.length())
				input_box->set_message(_renderer, "Введено не число");
			else
				input_box->set_message(_renderer, std::format("Число должно быть в диапазоне от 1 до {}", _max_amount));
			return;
		}
	}
	catch (std::logic_error& error)
	{
		button->bEnabled = false;
		if (dynamic_cast<std::out_of_range*>(&error) != nullptr and (pos == text.length() or text[0] >= '0' and text[0] <= '9'))
			input_box->set_message(_renderer, std::format("Число должно быть в диапазоне от 1 до {}", _max_amount));
		else if (not text.empty())
			input_box->set_message(_renderer, "Введено не число");
		return;
	}
	input_box->set_message();
	_strings_amount = number;
	button->bEnabled = true;
}
