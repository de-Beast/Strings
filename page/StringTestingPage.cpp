#include "StringTestingPage.h"

#include "StringMenuPage.h"

#include "../Globals.h"

template <>
StringTestingPage<BaseString>::StringTestingPage(SDL_Renderer* const renderer)
	: StringTestingPageBase(renderer)
{
	StringTestingPage::create_string_buttons();
	StringTestingPage::init_bind_buttons();
}


template <>
StringTestingPage<IdentifierString>::StringTestingPage(SDL_Renderer* const renderer)
	: StringTestingPageBase(renderer)
{
	const auto text_box = std::dynamic_pointer_cast<TextBox>(_ui_elems.at("LengthMethodText"));

	Dimensions dims(250, 30);
	SDL_Point coords(text_box->X, text_box->Y + dims.h + 20);
	constexpr int font_size = 25;
	constexpr int pixels_alignment = 20;

	auto button = create_object<Button>("FindCharMethodButton", dims, coords, "FindChar()", font_size, true);
	create_object<InputBox>("FindCharMethodInput", {30, 35}, {coords.x + button->W + pixels_alignment, coords.y}, 30)->bVisible = false;
	coords.y += dims.h + 20;

	Dimensions dims2(75, 30);
	constexpr int cancel_font_size = 20;
	create_object<Button>("FindCharMethodCancelButton", dims2, {button->X - dims2.w - pixels_alignment, button->Y}, "Отмена", cancel_font_size)->bVisible = false;

	std::dynamic_pointer_cast<Button>(_ui_elems.at("AssignmentOperatorButton"))->move(coords);
	std::dynamic_pointer_cast<Button>(_ui_elems.at("CancelButton0"))->move({-1, coords.y});
	coords.y += dims.h + 20;
	
	button = create_object<Button>("BiggerOperatorButton", dims, coords, "operator>", font_size, true);
	create_object<Button>("CancelButton1", dims2, {button->X - dims2.w - pixels_alignment, button->Y}, "Отмена", cancel_font_size)->bVisible = false;
	coords.y += dims.h + 20;

	button = create_object<Button>("BiggerOrEqualOperatorButton", dims, coords, "operator>=", font_size, true);
	create_object<Button>("CancelButton2", dims2, {button->X - dims2.w - pixels_alignment, button->Y}, "Отмена", cancel_font_size)->bVisible = false;

	StringTestingPage::create_string_buttons();
	StringTestingPage::init_bind_buttons();
}


template <>
void StringTestingPage<IdentifierString>::process_text_input(const SDL_Event& event)
{
	const auto input_box = std::dynamic_pointer_cast<InputBox>(_ui_elems.at("FindCharMethodInput"));
	input_box->process_text_input(event, _renderer);

	_ui_elems.erase("FindCharMethodResult");
	constexpr Dimensions dims(250, 30);
	if (not input_box->Text.empty())
		create_object<TextBox>("FindCharMethodResult", dims, {input_box->X + input_box->W + 20, input_box->Y}, std::string("Position: ") + std::to_string(dynamic_cast<IdentifierString*>((*_db)[_db->Test_index])->find_char(input_box->Text.back())), 25, true);
}

template <class Str> requires std::derived_from<Str, BaseString>
void StringTestingPage<Str>::process_text_input(const SDL_Event& event)
{
	Page::process_text_input(event);
}

template <>
StringTestingPage<HexString>::StringTestingPage(SDL_Renderer* const renderer)
	: StringTestingPageBase(renderer)
{
	const auto text_box = std::dynamic_pointer_cast<TextBox>(_ui_elems.at("LengthMethodText"));

	Dimensions dims(250, 30);
	SDL_Point coords(text_box->X, text_box->Y + dims.h + 20);
	constexpr int font_size = 25;

	create_object<TextBox>("ToIntMethodText", dims, coords, "<- ToInt()", font_size, true);
	coords.y += dims.h + 20;
	create_object<TextBox>("IsPositiveMethodText", dims, coords, "<- IsPositive()", font_size, true);
	coords.y += dims.h + 20;

	std::dynamic_pointer_cast<Button>(_ui_elems.at("AssignmentOperatorButton"))->move(coords);
	std::dynamic_pointer_cast<Button>(_ui_elems.at("CancelButton0"))->move({-1, coords.y});
	coords.y += dims.h + 20;

	Dimensions dims2(75, 30);
	constexpr int cancel_font_size = 20;
	constexpr int pixels_alignment = 20;

	auto button = create_object<Button>("EqualityOperatorButton", dims, coords, "operator==", font_size, true);
	create_object<Button>("CancelButton1", dims2, {button->X - dims2.w - pixels_alignment, button->Y}, "Отмена", cancel_font_size)->bVisible = false;
	coords.y += dims.h + 20;

	button = create_object<Button>("AdditionOperatorButton", dims, coords, "operator+", font_size, true);
	create_object<Button>("CancelButton2", dims2, {button->X - dims2.w - pixels_alignment, button->Y}, "Отмена", cancel_font_size)->bVisible = false;

	StringTestingPage::create_methods_text_boxes();
	StringTestingPage::create_string_buttons();
	StringTestingPage::init_bind_buttons();
}

template <>
void StringTestingPage<HexString>::create_methods_text_boxes()
{
	constexpr Dimensions dims(100, 30);
	constexpr int pixels_alignment = 20;

	auto text_box1 = std::dynamic_pointer_cast<TextBox>(_ui_elems.at("ToIntMethodText"));
	_ui_elems.erase("ToIntMethodResultText");
	auto text_box2 = create_object<TextBox>("ToIntMethodResultText", dims, {text_box1->X - pixels_alignment, text_box1->Y}, std::to_string(dynamic_cast<HexString*>((*_db)[_db->get_test_index()])->to_int()), 25, true);
	text_box2->move({text_box2->X - text_box2->W, text_box2->Y});

	text_box1 = std::dynamic_pointer_cast<TextBox>(_ui_elems.at("IsPositiveMethodText"));
	_ui_elems.erase("IsPositiveMethodResultText");
	text_box2 = create_object<TextBox>("IsPositiveMethodResultText", dims, {text_box1->X - pixels_alignment, text_box1->Y}, dynamic_cast<HexString*>((*_db)[_db->get_test_index()])->is_positive() ? "True" : "False", 25, true);
	text_box2->move({text_box2->X - text_box2->W, text_box2->Y});
}



template <class Str> requires std::derived_from<Str, BaseString>
void StringTestingPage<Str>::create_string_buttons()
{
	if (_strings_amount > 0)
		return;

	const size_t size = StringDataBase::size();
	for (size_t i = 0, k = 0; i < size; i++)
	{
		const size_t test_index = _db->get_test_index();
		if (i == test_index)
			continue;

		if (_db->get_string_data(test_index).str_type != _db->get_string_data(i).str_type)
			continue;

		Dimensions dims(250, 30);
		SDL_Point coords(WINDOW_WIDTH - dims.w - 100, 100 + _strings_amount * (dims.h + 20));
		const auto button = create_object<Button>("StringButton" + std::to_string(k++) , dims, coords, (*_db)[i]->c_str(), 30, true);
		button->bind_on_click(&string_button, std::make_shared<StringIndexArgs>(button, i));
		button->bVisible = false;
		_strings_amount++;
	}
}


template <class Str> requires std::derived_from<Str, BaseString>
void StringTestingPage<Str>::create_methods_text_boxes()
{
	StringTestingPageBase::create_methods_text_boxes();
}

template <>
void StringTestingPage<BaseString>::string_button(const EventArguments& args)
{
	const auto casted_args = std::static_pointer_cast<StringIndexArgs>(args);
	using enum StringDataBase::ETestOperator;
	switch (_db->Test_operator)
	{
		case ASSIGNMENT:
		{
			*(*_db)[_db->Test_index] = *(*_db)[casted_args->string_index];
			const auto page = get_page<StringTestingPage>();
			page->create_string_text_box();
			page->create_methods_text_boxes();
			break;
		}
	}
}


template <>
void StringTestingPage<IdentifierString>::string_button(const EventArguments& args)
{
	const auto casted_args = std::static_pointer_cast<StringIndexArgs>(args);
	const auto button = casted_args->button.lock();
	if (not button)
		return;

	using enum StringDataBase::ETestOperator;
	switch (_db->Test_operator)
	{
		case ASSIGNMENT:
		{
			*(*_db)[_db->Test_index] = *(*_db)[casted_args->string_index];
			const auto page = get_page<StringTestingPage>();
			cancel_button(std::make_shared<StringMethodArgs>(get_object<Button>("FindCharMethodCancelButton"), true));
			page->create_string_text_box();
			page->create_methods_text_boxes();
			break;
		}
		case BIGGER:
		case BIGGER_OR_EQUAL:
		{
			const auto page = get_page<StringTestingPage>();
			constexpr Dimensions dims(210, 30);
			const SDL_Point coords(button->X + button->W + 20, button->Y);
			const int string_amount = get_page<StringTestingPage<IdentifierString>>()->_strings_amount;
			int index = 0;
			while (index < string_amount)
			{
				try
				{
					if (_db->Test_operator == BIGGER)
						page->create_object<TextBox>("BiggerOperatorResult" + std::to_string(index), dims, coords, *dynamic_cast<IdentifierString*>((*_db)[_db->Test_index]) > *dynamic_cast<IdentifierString*>((*_db)[casted_args->string_index]) ? "-> True" : "-> False", 25, true);
					else
						page->create_object<TextBox>("BiggerOrEqualOperatorResult" + std::to_string(index), dims, coords, *dynamic_cast<IdentifierString*>((*_db)[_db->Test_index]) >= *dynamic_cast<IdentifierString*>((*_db)[casted_args->string_index]) ? "-> True" : "-> False", 25, true);
					break;
				}
				catch(const std::runtime_error&)
				{
					index++;
				}
			}
			break;
		}
	}
}


template <>
void StringTestingPage<HexString>::string_button(const EventArguments& args)
{
	const auto casted_args = std::static_pointer_cast<StringIndexArgs>(args);
	const auto button = casted_args->button.lock();
	if (not button)
		return;

	using enum StringDataBase::ETestOperator;
	switch (_db->Test_operator)
	{
		case ASSIGNMENT:
		{
			*(*_db)[_db->Test_index] = *(*_db)[casted_args->string_index];
			const auto page = get_page<StringTestingPage>();
			cancel_button(std::make_shared<StringMethodArgs>(get_object<Button>("FindCharMethodCancelButton"), true));
			page->create_string_text_box();
			page->create_methods_text_boxes();
			break;
		}
		case EQUALITY:
		case ADDITION:
		{
			const auto page = get_page<StringTestingPage>();
			constexpr Dimensions dims(210, 30);
			const SDL_Point coords(button->X + button->W + 20, button->Y);
			const int string_amount = get_page<StringTestingPage<HexString>>()->_strings_amount;
			int index = 0;
			while (index < string_amount)
			{
				try
				{
					if (_db->Test_operator == EQUALITY)
						page->create_object<TextBox>("EqualityOperatorResult" + std::to_string(index), dims, coords, *dynamic_cast<HexString*>((*_db)[_db->Test_index]) == *dynamic_cast<HexString*>((*_db)[casted_args->string_index]) ? "-> True" : "-> False", 25, true);
					else
						page->create_object<TextBox>("AdditionOperatorResult" + std::to_string(index), dims, coords, std::string("-> ") + std::string((*dynamic_cast<HexString*>((*_db)[_db->Test_index]) + *dynamic_cast<HexString*>((*_db)[casted_args->string_index])).c_str()), 25, true);
					break;
				}
				catch (const std::runtime_error&)
				{
					index++;
				}
			}
		}
	}
}

template <>
void StringTestingPage<IdentifierString>::find_char_button(const EventArguments&)
{
	get_object<InputBox>("FindCharMethodInput")->bVisible = true;
	get_object<Button>("FindCharMethodCancelButton")->bVisible = true;
}

template <class Str> requires std::derived_from<Str, BaseString>
void StringTestingPage<Str>::cancel_button(const EventArguments& args)
{
	if (const auto casted_args = std::static_pointer_cast<StringMethodArgs>(args);
		casted_args->is_find_char)
	{
		const auto input_box = get_object<InputBox>("FindCharMethodInput");
		input_box->bVisible = false;
		input_box->set_text(get_page< StringTestingPage<Str>>()->_renderer);
		const auto cancel_button = casted_args->button.lock();
		if (cancel_button != nullptr)
			cancel_button->bVisible = false;
		get_page<StringTestingPage<Str>>()->_ui_elems.erase("FindCharMethodResult");
	}
	else
	{
		get_page<StringTestingPage>()->erase_oparators_results();
		StringTestingPageBase::cancel_button(args);
	}
}

template <class Str> requires std::derived_from<Str, BaseString>
void StringTestingPage<Str>::operator_method_button(const EventArguments& args)
{
	StringTestingPageBase::operator_method_button(args);
	get_page<StringTestingPage>()->erase_oparators_results();
}

template <>
void StringTestingPage<IdentifierString>::init_bind_buttons()
{
	StringTestingPageBase::init_bind_buttons();

	std::shared_ptr<Button> button;

	if (button = std::dynamic_pointer_cast<Button>(_ui_elems.at("FindCharMethodButton"));
		button != nullptr)
	{
		button->bind_on_click(&find_char_button);
	}

	if (button = std::dynamic_pointer_cast<Button>(_ui_elems.at("BiggerOperatorButton"));
		button != nullptr)
	{
		button->bind_on_click(&operator_method_button, std::make_shared<TestMethodArgs>(StringDataBase::ETestOperator::BIGGER, 1));
	}

	if (button = std::dynamic_pointer_cast<Button>(_ui_elems.at("BiggerOrEqualOperatorButton"));
		button != nullptr)
	{
		button->bind_on_click(&operator_method_button, std::make_shared<TestMethodArgs>(StringDataBase::ETestOperator::BIGGER_OR_EQUAL, 2));
	}

	if (button = std::dynamic_pointer_cast<Button>(_ui_elems.at("FindCharMethodCancelButton"));
		button != nullptr)
	{
		button->bind_on_click(&cancel_button, std::make_shared<StringMethodArgs>(button, true));
	}
}

template <>
void StringTestingPage<HexString>::init_bind_buttons()
{
	StringTestingPageBase::init_bind_buttons();

	std::shared_ptr<Button> button;

	if (button = std::dynamic_pointer_cast<Button>(_ui_elems.at("EqualityOperatorButton"));
		button != nullptr)
	{
		button->bind_on_click(&operator_method_button, std::make_shared<TestMethodArgs>(StringDataBase::ETestOperator::EQUALITY, 1));
	}

	if (button = std::dynamic_pointer_cast<Button>(_ui_elems.at("AdditionOperatorButton"));
		button != nullptr)
	{
		button->bind_on_click(&operator_method_button, std::make_shared<TestMethodArgs>(StringDataBase::ETestOperator::ADDITION, 2));
	}
}

template <class Str> requires std::derived_from<Str, BaseString>
void StringTestingPage<Str>::init_bind_buttons()
{
	StringTestingPageBase::init_bind_buttons();
}