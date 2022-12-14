#include "StringMenuPage.h"

#include <iostream>
#include <ranges>

#include "../Globals.h"
#include "../UI.h"
#include "../string/StringDataBase.h"
#include "MainMenuPage.h"
#include "StringTestingPage.h"

StringMenuPage::StringMenuPage(SDL_Renderer* renderer)
	: Page(renderer)
{
	_db = &StringDataBase::get_instance();

	auto size = StringDataBase::size();
	Dimensions dims(300, 40);
	SDL_Point coords((WINDOW_WIDTH - dims.w) / 3, (WINDOW_HEIGHT - dims.h)/ 5);
	for (size_t i = 0; i < size; i++)
	{
		const auto input_box = create_object<InputBox>("StringInput" + std::to_string(i), dims, coords, 30);
		input_box->bEnabled = false;

		Dimensions dims2(110, dims.h);
		SDL_Point coords2(coords.x + dims.w + 10, coords.y);
		create_object<Button>("MakeButton" + std::to_string(i), dims2, coords2, "Создать", 25);

		coords2 = SDL_Point(coords2.x + dims2.w + 10, coords.y);
		dims2 = Dimensions(180, dims.h);
		auto button = create_object<Button>("TestButton" + std::to_string(i), dims2, coords2, "Тестировать", 25);

		if(not (*_db)[i]->is_empty())
		{
			input_box->set_text(renderer, (*_db)[i]->c_str());
			set_string_type_message(std::to_string(i), Colors::WHITE);
		}
		else
			button->bEnabled = false;

		dims2 = Dimensions(120, dims.h);
		coords2 = SDL_Point(coords.x - dims2.w - 10, coords.y);
		button = create_object<Button>("CancelButton" + std::to_string(i), dims2, coords2, "Отмена", 25);
		button->bVisible = false;

		coords.y += dims.h + 20;
	}
	
	dims = Dimensions(120, 30);
	create_object<Button>("BackButton", dims, SDL_Point(30, WINDOW_HEIGHT - 2 * dims.h), "Назад", 30);

	StringMenuPage::init_bind_buttons();
}

void StringMenuPage::process_text_input(const SDL_Event& event)
{
	for (auto& box : _ui_elems | std::views::values)
	{
		if (std::shared_ptr<InputBox> input_box = std::dynamic_pointer_cast<InputBox>(box);
			input_box != nullptr and input_box->is_focused())
		{
			input_box->process_text_input(event, _renderer);
			check_input(input_box);
			break;
		}
	}
}

void StringMenuPage::retrieve_input_box_text(const ID& id, const std::string& index) const
{
	const auto input_box = get_object<InputBox>(id);
	input_box->set_text(_renderer, (*_db)[std::stoi(index)]->c_str());
}

void StringMenuPage::back_button(const EventArguments&)
{
	init_page<MainMenuPage>(get_page<StringMenuPage>()->_renderer);
}

void StringMenuPage::make_string_button(const EventArguments& args)
{
	const auto button{std::static_pointer_cast<ButtonArgs>(args)->button.lock()};
	if (button == nullptr)
		return;

	const auto index = std::string{button->Id.back()};
	const ID id = "StringTypeButton" + index;

	const auto make_button = get_object<Button>("MakeButton" + index);
	make_button->bVisible = false;
	const auto cancel_button = get_object<Button>("CancelButton" + index);
	cancel_button->bind_on_click(std::make_shared<CancelTypeArgs>(cancel_button, CancelTypeArgs::ECancelType::STRING_TYPE_SELECTION_STATE));
	cancel_button->bVisible = true;
	get_object<Button>("TestButton" + index)->bVisible = false;

	const std::vector string_types = {"String", "Identifier String", "HexString"};
	const std::vector<Dimensions> dims_vec = {{80, button->H}, {120, button->H}, {120, button->H}};
	SDL_Point coords(button->X, button->Y);
	for (int i = 0; i < 3; i++)
	{
		Dimensions dims = dims_vec[i];
		const auto new_button = get_page<StringMenuPage>()->create_object<Button>(id + std::to_string(i), dims, coords, string_types[i], 20);
		switch (i)
		{
			case 0:	new_button->bind_on_click(string_type_button<BaseString>, std::make_shared<ButtonArgs>(make_button)); break;
			case 1:	new_button->bind_on_click(string_type_button<IdentifierString>, std::make_shared<ButtonArgs>(make_button)); break;
			case 2:	new_button->bind_on_click(string_type_button<HexString>, std::make_shared<ButtonArgs>(make_button)); break;
		}
		new_button->move({new_button->X, new_button->Y - (new_button->H - dims.h) / 2});
		coords.x += dims.w + 20;
	}
}

void StringMenuPage::confirm_construct_button(const EventArguments& args)
{
	const auto clicked_button = std::static_pointer_cast<ButtonArgs>(args)->button.lock();
	if (clicked_button == nullptr)
		return;

	const auto index = std::string{clicked_button->Id.back()};
	BaseString* string = create_string(std::stoi(index), get_object<InputBox>("StringInput" + index)->Text);
	_db->set_string(std::stoi(index), string);

	get_object<Button>("TestButton" + index)->bEnabled = true;
	const auto cancel_button = get_object<Button>("CancelButton" + index);
	for (int i = 0; i < 3; i++)
		cancel_button->on_click();
}


void StringMenuPage::constructor_type_button(const EventArguments& args)
{
	const auto clicked_button = std::static_pointer_cast<ButtonArgs>(args)->button.lock();
	if (clicked_button == nullptr)
		return;

	const auto index = std::string{*(clicked_button->Id.end() - 2)};

	const ID id = "ConstructorButton" + index;
	using enum StringDataBase::StringData::EConstructorType;
	switch (clicked_button->Id.back())
	{
		case '0': _db->set_constructor_type(std::stoi(index), C_STR); break;
		case '1': _db->set_constructor_type(std::stoi(index), COPY); break;
		case '2': _db->set_constructor_type(std::stoi(index), CHAR);	break;
	}

	try
	{
		for (size_t i = 0; i < 3; i++)
			get_object<Button>(id + std::to_string(i))->bVisible = false;
	}
	catch (std::out_of_range&){}

	if (clicked_button->Id.back() != '1')
	{
		const auto left_aligned_button = get_object<Button>(id + '0');
		const Dimensions dims(110, left_aligned_button->H);
		const auto confirm_button = get_page<StringMenuPage>()->create_object<Button>("ConfirmButton" + index, dims, {left_aligned_button->X,left_aligned_button->Y}, "Принять", 25);
		confirm_button->bind_on_click(confirm_construct_button, std::make_shared<ButtonArgs>(confirm_button));

		get_object<InputBox>("StringInput" + index)->bEnabled = true;
		check_input(get_object<InputBox>("StringInput" + index));

		const auto cancel_button = get_object<Button>("CancelButton" + index);
		cancel_button->bind_on_click(std::make_shared<CancelTypeArgs>(cancel_button, CancelTypeArgs::ECancelType::STRING_INPUT_STATE));
	}
	else
	{
		std::stack<ID> ids;
		const size_t size = StringDataBase::size();
		for (size_t i = 0; i < size; i++)
		{
			if (i == std::stoul(index))
				continue;

			const auto cancel_button = get_object<Button>("CancelButton" + std::to_string(i));
			const auto cancel_args = std::static_pointer_cast<CancelTypeArgs>(cancel_button->get_args());
			if (cancel_button != nullptr and cancel_button->bVisible)
			{
				cancel_button->bVisible = false;
				ids.push(cancel_button->Id);
			}
			if (cancel_args != nullptr)
			{
				using enum CancelTypeArgs::ECancelType;
				switch (cancel_args->type)
				{
					case STRING_TYPE_SELECTION_STATE:
					{
						for (int j = 0; j < 3; j++)
						{
							const ID local_id = "StringTypeButton" + std::to_string(i) + std::to_string(j);
							get_object<Button>(local_id)->bVisible = false;
							ids.push(local_id);
						}
						break;
					}
					case CONSTRUCTOR_SELECTION_STATE:
					{
						for (int j = 0; j < 3; j++)
						{
							const ID local_id = "ConstructorButton" + std::to_string(i) + std::to_string(j);
							try
							{
								get_object<Button>(local_id)->bVisible = false;
								ids.push(local_id);
							}
							catch (std::out_of_range&)
							{
								break;
							}
						}
						break;
					}
					case STRING_INPUT_STATE:
					{
						ID local_id = "StringInput" + std::to_string(i);
						get_object<InputBox>(local_id)->bEnabled = false;
						ids.push(local_id);

						local_id = "ConfirmButton" + std::to_string(i);
						get_object<Button>(local_id)->bVisible = false;
						ids.push(local_id);
						break;
					}
				}
			}
			else
			{
				ID local_id = "MakeButton" + std::to_string(i);
				get_object<Button>(local_id)->bVisible = false;
				ids.push(local_id);

				local_id = "TestButton" + std::to_string(i);
				get_object<Button>(local_id)->bVisible = false;
				ids.push(local_id);
			}

			if (_db->get_string_data(i).str_type == _db->get_string_data(std::stoul(index)).str_type and not (*_db)[i]->is_empty())
			{
				const ID local_id = "SelectStringButton" + std::to_string(i);
				Dimensions dims(110, 30);
				const auto& left_aligned_button = get_object<Button>("MakeButton" + std::to_string(i));
				SDL_Point coords(left_aligned_button->X, left_aligned_button->Y);
				const auto new_button = get_page<StringMenuPage>()->create_object<Button>(local_id, dims, coords, "Выбрать", 25);
				ids.push(local_id);
				using enum StringDataBase::StringData::EStringType;
				switch (_db->get_string_data(i).str_type)
				{
					case BASE_STRING: new_button->bind_on_click(select_string_button<BaseString>, std::make_shared<InputBoxAndButtonArgs>(new_button, get_object<InputBox>("StringInput" + index))); break;
					case IDENTIFIER_STRING: new_button->bind_on_click(select_string_button<IdentifierString>, std::make_shared<InputBoxAndButtonArgs>(new_button, get_object<InputBox>("StringInput" + index))); break;
					case HEX_STRING: new_button->bind_on_click(select_string_button<HexString>, std::make_shared<InputBoxAndButtonArgs>(new_button, get_object<InputBox>("StringInput" + index))); break;
				}
			}
		}

		const auto cancel_button = get_object<Button>("CancelButton" + index);
		cancel_button->bind_on_click(std::make_shared<IdsAndCancelTypeArgs>(cancel_button, CancelTypeArgs::ECancelType::COPY_STRING_STATE, ids));
	}
}


void StringMenuPage::test_button(const EventArguments& args)
{
	const auto button{std::static_pointer_cast<ButtonArgs>(args)->button.lock()};
	if (button == nullptr)
		return;
	const int index = std::stoi(std::string{button->Id.back()});
		
	_db->Test_index = index;

	using enum StringDataBase::StringData::EStringType;
	switch (_db->get_string_data(index).str_type)
	{
		case BASE_STRING: init_page<StringTestingPage<BaseString>>(get_page<StringMenuPage>()->_renderer); break;
		case IDENTIFIER_STRING: init_page<StringTestingPage<IdentifierString>>(get_page<StringMenuPage>()->_renderer); break;
		case HEX_STRING: init_page<StringTestingPage<HexString>>(get_page<StringMenuPage>()->_renderer); break;
	}
	
}

void StringMenuPage::cancel_button(const EventArguments& args)
{
	const auto cancel_button{std::static_pointer_cast<CancelTypeArgs>(args)->button.lock()};
	const auto cancel_type = std::static_pointer_cast<CancelTypeArgs>(args)->type;
	if (cancel_button == nullptr)
		return;

	const auto index = std::string{cancel_button->Id.back()};

	using enum CancelTypeArgs::ECancelType;
	switch (cancel_type)
	{
		case STRING_TYPE_SELECTION_STATE:
		{

			const ID id = "StringTypeButton" + index;
			get_object<Button>("MakeButton" + index)->bVisible = true;
			get_object<Button>("TestButton" + index)->bVisible = true;
			get_object<Button>("CancelButton" + index)->bVisible = false;

			for (int i = 0; i < 3; i++)
			{
				get_page<StringMenuPage>()->_ui_elems.erase(id + std::to_string(i));
			}
			cancel_button->bind_on_click(EventArguments(nullptr));
			break;
		}
		case CONSTRUCTOR_SELECTION_STATE: 
		{
			for (size_t i = 0; i < 3; i++)
				get_object<Button>("StringTypeButton" + index + std::to_string(i))->bVisible = true;

			const ID id = "ConstructorButton" + index;
			for (int i = 0; i < 3; i++)
			{
				get_page<StringMenuPage>()->_ui_elems.erase(id + std::to_string(i));
			}
			cancel_button->bind_on_click(std::make_shared<CancelTypeArgs>(cancel_button, CancelTypeArgs::ECancelType::STRING_TYPE_SELECTION_STATE));

			if (get_object<InputBox>("StringInput" + index)->Text.empty())
			{
				_db->set_string_type(std::stoul(index), StringDataBase::StringData::EStringType::EMPTY);
			}
			else if (dynamic_cast<IdentifierString*>((*_db)[std::stoul(index)]) != nullptr)
			{
				_db->set_string_type(std::stoul(index), StringDataBase::StringData::EStringType::IDENTIFIER_STRING);
			}
			else if (dynamic_cast<HexString*>((*_db)[std::stoul(index)]) != nullptr)
			{
				_db->set_string_type(std::stoul(index), StringDataBase::StringData::EStringType::HEX_STRING);
			}
			else
			{
				_db->set_string_type(std::stoul(index), StringDataBase::StringData::EStringType::BASE_STRING);
			}
			get_page<StringMenuPage>()->set_string_type_message(index, Colors::WHITE);
			break;
		}
		case STRING_INPUT_STATE:
		{
			const ID id = "ConstructorButton" + index;
			try
			{
				for (size_t i = 0; i < 3; i++)
					get_object<Button>(id + std::to_string(i))->bVisible = true;
			}
			catch(std::out_of_range&){}

			get_page<StringMenuPage>()->_ui_elems.erase("ConfirmButton" + index);

			cancel_button->bind_on_click(std::make_shared<CancelTypeArgs>(cancel_button, CancelTypeArgs::ECancelType::CONSTRUCTOR_SELECTION_STATE));
			get_object<InputBox>("StringInput" + index)->bEnabled = false;

			get_page<StringMenuPage>()->retrieve_input_box_text("StringInput" + index, index);
			break;
		}
		case COPY_STRING_STATE:
		{
			std::stack<ID>& ids{std::static_pointer_cast<IdsAndCancelTypeArgs>(args)->ids};
			while (not ids.empty())
			{
				const ID id = ids.top();
				if (const auto ui_elem = get_object<UIElement>(id);
					not ui_elem->bVisible)
				{
					ui_elem->bVisible = true;
				}
				else if (auto clickable = std::dynamic_pointer_cast<IClickable>(ui_elem); 
						 not clickable->bEnabled)
				{
					clickable->bEnabled = true;
				}
				else
				{
					get_page<StringMenuPage>()->_ui_elems.erase(id);
				}

				ids.pop();
			}

			const ID id = "ConstructorButton" + index;
			try
			{
				for (size_t i = 0; i < 3; i++)
					get_object<Button>(id + std::to_string(i))->bVisible = true;
			}
			catch (std::out_of_range&){}

			cancel_button->bind_on_click(std::make_shared<CancelTypeArgs>(cancel_button, CancelTypeArgs::ECancelType::CONSTRUCTOR_SELECTION_STATE));
			break;
		}
	}
}

BaseString* StringMenuPage::create_string(const size_t index, const std::string& string)
{
	const auto string_type = _db->get_string_data(index).str_type;
	using enum StringDataBase::StringData::EStringType;
	switch (string_type)
	{
		case BASE_STRING: return construct_string<BaseString>(index, string);
		case IDENTIFIER_STRING: return construct_string<IdentifierString>(index, string);
		case HEX_STRING: return construct_string<HexString>(index, string);
	}
	return nullptr;
}

void StringMenuPage::set_string_type_message(const std::string& index, const SDL_Color& color) const
{
	std::string message;
	using enum StringDataBase::StringData::EStringType;
	switch (_db->get_string_data(std::stoul(index)).str_type)
	{
		case BASE_STRING: message = "BaseString"; break;
		case IDENTIFIER_STRING: message = "IdentifierString"; break;
		case HEX_STRING: message = "HexString"; break;
	}
	const auto input_box = std::dynamic_pointer_cast<InputBox>(_ui_elems.at("StringInput" + index));
	input_box->set_message(_renderer, message, false, 15, color, {input_box->W, 20});
}

void StringMenuPage::check_input(const std::shared_ptr<InputBox>& input_box)
{
	const auto index = std::string{input_box->Id.back()};
	const BaseString* string = create_string(std::stoi(index), input_box->Text);
	if (string != nullptr and std::strcmp(string->c_str(), input_box->Text.c_str()) == 0)
		get_object<Button>("ConfirmButton" + index)->bEnabled = true;
	else
		get_object<Button>("ConfirmButton" + index)->bEnabled = false;
	delete string;
}

void StringMenuPage::init_bind_buttons()
{
	std::shared_ptr<Button> button;
	const size_t size = StringDataBase::size();

	for (size_t i = 0; i < size; i++)
	{
		if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("MakeButton" + std::to_string(i)))) != nullptr)
			button->bind_on_click(&make_string_button, std::make_shared<ButtonArgs>(button));

		if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("TestButton" + std::to_string(i)))) != nullptr)
			button->bind_on_click(&test_button, std::make_shared<ButtonArgs>(button));

		if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("CancelButton" + std::to_string(i)))) != nullptr)
			button->bind_on_click(&cancel_button);
	}

	if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("BackButton"))) != nullptr)
		button->bind_on_click(&back_button);
}
