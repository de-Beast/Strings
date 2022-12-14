#include "../Globals.h"
#include "StringTestingPageBase.h"

#include "StringMenuPage.h"


StringTestingPageBase::StringTestingPageBase(SDL_Renderer* const renderer)
	: Page(renderer)
{
	_db = &StringDataBase::get_instance();
	create_string_text_box();

	Dimensions dims(210, 30);
	SDL_Point coords(120, 100);
	constexpr int font_size = 25;
	constexpr int cancel_font_size = 20;
	constexpr int pixels_alignment = 20;
	create_object<TextBox>("LengthMethodText", dims, coords, "<- Length()", font_size, true);

	coords.y += dims.h + 20;
	const auto button = create_object<Button>("AssignmentOperatorButton", dims, coords, "operator=", font_size, true);
	Dimensions dims2(75, 30);
	create_object<Button>("CancelButton0", dims2, {button->X - dims2.w - pixels_alignment, button->Y}, "Отмена", cancel_font_size)->bVisible = false;

	dims = {120, 30};
	create_object<Button>("BackButton", dims, SDL_Point(30, WINDOW_HEIGHT - 2 * dims.h), "Назад", 30);

	StringTestingPageBase::create_methods_text_boxes();
}

void StringTestingPageBase::cancel_button(const EventArguments& args)
{
	const auto cancel_button = std::static_pointer_cast<ButtonArgs>(args)->button.lock();
	if (cancel_button != nullptr)
		cancel_button->bVisible = false;

	show_string_buttons(false);
}

void StringTestingPageBase::operator_method_button(const EventArguments& args)
{
	const auto casted_args = std::static_pointer_cast<TestMethodArgs>(args);
	_db->Test_operator = casted_args->test_operator;
	try
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == casted_args->cancel_button_index)
				get_object<Button>("CancelButton" + std::to_string(i))->bVisible = true;
			else
				get_object<Button>("CancelButton" + std::to_string(i))->bVisible = false;
		}
	}
	catch (const std::out_of_range&){}
	get_page<StringTestingPageBase>()->erase_oparators_results();
	show_string_buttons(true);
}

void StringTestingPageBase::show_string_buttons(const bool do_show)
{
	const int amount = get_page<StringTestingPageBase>()->_strings_amount;
	for (int i = 0; i < amount; i++)
	{
		get_object<Button>("StringButton" + std::to_string(i))->bVisible = do_show;
	}
}

void StringTestingPageBase::erase_oparators_results()
{
	for (int i = 0; i < _strings_amount; i++)
	{
		_ui_elems.erase("BiggerOperatorResult" + std::to_string(i));
		_ui_elems.erase("BiggerOrEqualOperatorResult" + std::to_string(i));
		_ui_elems.erase("EqualityOperatorResult" + std::to_string(i));
		_ui_elems.erase("AdditionOperatorResult" + std::to_string(i));
	}
}

void StringTestingPageBase::create_methods_text_boxes()
{
	constexpr Dimensions dims(100, 30);

	const auto length_text_box = std::dynamic_pointer_cast<TextBox>(_ui_elems.at("LengthMethodText"));
	_ui_elems.erase("LengthMethodResultText");
	const auto text_box = create_object<TextBox>("LengthMethodResultText", dims, {length_text_box->X - 20, length_text_box->Y}, std::to_string((*_db)[_db->Test_index]->length()), 25, true);
	text_box->move({text_box->X - text_box->W, text_box->Y});
}

void StringTestingPageBase::back_button(const EventArguments&)
{
	init_page<StringMenuPage>(get_page<StringTestingPageBase>()->_renderer);
}

void StringTestingPageBase::create_string_text_box()
{
	_ui_elems.erase("StringText");

	constexpr Dimensions dims(500, 50);
	std::string string_type;
	using enum StringDataBase::StringData::EStringType;
	switch (_db->get_string_data(_db->Test_index).str_type)
	{
		case BASE_STRING: string_type = "Base String: "; break;
		case IDENTIFIER_STRING: string_type = "Identifier String: "; break;
		case HEX_STRING: string_type = "Hex String: "; break;
	}
	create_object<TextBox>("StringText", dims, {(WINDOW_WIDTH - dims.w) / 2, 10}, string_type + (*_db)[_db->Test_index]->c_str(), 30);
}

void StringTestingPageBase::init_bind_buttons()
{
	std::dynamic_pointer_cast<Button>(_ui_elems.at("BackButton"))->bind_on_click(&back_button);
	std::dynamic_pointer_cast<Button>(_ui_elems.at("AssignmentOperatorButton"))->bind_on_click(&operator_method_button, std::make_shared<TestMethodArgs>(StringDataBase::ETestOperator::ASSIGNMENT, 0));
	try
	{
		for (int i = 0; i < 3; i++)
		{
			const auto button = std::dynamic_pointer_cast<Button>(_ui_elems.at("CancelButton" + std::to_string(i)));
			button->bind_on_click(&cancel_button, std::make_shared<StringMethodArgs>(button, false));
		}
	}
	catch (const std::out_of_range&){}
}
