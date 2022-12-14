#pragma once
#include <stack>

#include "Page.h"
#include "../UI/Button.h"
#include "../string/String.h"
#include "../string/StringDataBase.h"

class StringMenuPage : public Page
{
public:
	explicit StringMenuPage(SDL_Renderer* renderer);

	void process_text_input(const SDL_Event& event) override;

private:
	struct CancelTypeArgs : ButtonArgs
	{
		enum class ECancelType
		{
			STRING_TYPE_SELECTION_STATE,
			CONSTRUCTOR_SELECTION_STATE,
			STRING_INPUT_STATE,
			COPY_STRING_STATE,
		} type;

		CancelTypeArgs(const std::shared_ptr<Button>& button, const ECancelType type)
			: ButtonArgs(button), type(type){}
	};

	struct IdsAndCancelTypeArgs : CancelTypeArgs
	{
		std::stack<ID> ids;

		IdsAndCancelTypeArgs(const std::shared_ptr<Button>& button, const ECancelType type, const std::stack<ID>& ids)
			: CancelTypeArgs(button, type), ids(ids){}
	};

	template<class T>
	static void string_type_button(const EventArguments& args);
	static void confirm_construct_button(const EventArguments& args);
	static void make_string_button(const EventArguments& args);
	static void constructor_type_button(const EventArguments& args);
	template <class Str> requires std::derived_from<Str, BaseString>
	static void select_string_button(const EventArguments& args);
	static void test_button(const EventArguments& args);
	static void cancel_button(const EventArguments& args);
	static void back_button(const EventArguments&);

	template<class T> requires std::derived_from<T, BaseString>
	static BaseString* construct_string(const size_t index, const std::string& string);
	static BaseString* create_string(const size_t index, const std::string& string);

	static void check_input(const std::shared_ptr<InputBox>& input_box);

	void set_string_type_message(const std::string& index, const SDL_Color& color) const;
	void retrieve_input_box_text(const ID& id, const std::string& index) const;
	void init_bind_buttons() override;

	inline static StringDataBase* _db = nullptr;
};

template<class T>
void StringMenuPage::string_type_button(const EventArguments& args)
{
	const auto left_aligned_button = std::static_pointer_cast<ButtonArgs>(args)->button.lock();
	if (left_aligned_button == nullptr)
		return;

	const auto index = std::string{left_aligned_button->Id.back()};
	const auto cancel_button = get_object<Button>("CancelButton" + index);
	cancel_button->bind_on_click(std::make_shared<CancelTypeArgs>(cancel_button, CancelTypeArgs::ECancelType::CONSTRUCTOR_SELECTION_STATE));

	for (size_t i = 0; i < 3; i++)
		get_object<Button>("StringTypeButton" + index + std::to_string(i))->set_visibility(false);

	const ID id = "ConstructorButton" + index;
	using enum StringDataBase::StringData::EStringType;

	size_t size = 2;
	if (std::is_same_v<T, BaseString>)
	{
		_db->set_string_type(std::stoi(index), BASE_STRING);
		size = 3;
	}
	else if (std::is_same_v<T, IdentifierString>)
		_db->set_string_type(std::stoi(index), IDENTIFIER_STRING);
	else if (std::is_same_v<T, HexString>)
		_db->set_string_type(std::stoi(index), HEX_STRING);

	SDL_Point coords(left_aligned_button->X, left_aligned_button->Y);
	const std::vector constructors_types = {"C-String", "Copy", "Char"};
	const std::vector<Dimensions> dims_vec = {{100, left_aligned_button->H}, {50, left_aligned_button->H}, {50, left_aligned_button->H}};
	for (size_t i = 0; i < size; i++)
	{
		const auto new_button = get_page<StringMenuPage>()->create_object<Button>(id + std::to_string(i), dims_vec[i], coords, constructors_types[i], 20);
		new_button->bind_on_click(constructor_type_button, std::make_shared<ButtonArgs>(new_button));
		new_button->move({new_button->X, new_button->Y - (new_button->H - dims_vec[i].h) / 2});
		coords.x += dims_vec[i].w + 20;
	}

	get_page<StringMenuPage>()->set_string_type_message(index, Colors::GREEN);
}

template <class Str> requires std::derived_from<Str, BaseString>
void StringMenuPage::select_string_button(const EventArguments& args)
{
	const auto clicked_button = std::static_pointer_cast<InputBoxAndButtonArgs>(args)->button.lock();
	const size_t num_index1 = std::stoul(std::string{clicked_button->Id.back()});

	const auto input_box = std::static_pointer_cast<InputBoxAndButtonArgs>(args)->input_box.lock();
	const size_t num_index2 = std::stoul(std::string{input_box->Id.back()});

	if (clicked_button == nullptr or input_box == nullptr)
		return;

	const auto selected_input_box = get_object<InputBox>("StringInput" + std::to_string(num_index1));

	input_box->set_text(get_page<StringMenuPage>()->_renderer, selected_input_box->Text);
	Str* string = dynamic_cast<Str*>((*_db)[num_index1]);
	if (string != nullptr)
		_db->set_string(num_index2, new Str(*string));

	get_object<Button>("TestButton" + std::to_string(num_index2))->bEnabled = true;
	const auto cancel_button = get_object<Button>("CancelButton" + std::to_string(num_index2));
	for (size_t i = 0; i < 3; i++)
		cancel_button->on_click();
}

template <>
inline BaseString* StringMenuPage::construct_string<BaseString>(const size_t index, const std::string& string)
{
	if (string.empty())
		return nullptr;

	const auto constr_type = _db->get_string_data(index).constr_type;
	using enum StringDataBase::StringData::EConstructorType;
	switch (constr_type)
	{
		case C_STR: return new BaseString(string.c_str());
		case CHAR: return new BaseString(string.back());
	}
	return nullptr;
}

template <class T> requires std::derived_from<T, BaseString>
BaseString* StringMenuPage::construct_string(const size_t index, const std::string& string)
{
	if (string.empty())
		return nullptr;

	const auto constr_type = _db->get_string_data(index).constr_type;
	using enum StringDataBase::StringData::EConstructorType;
	if (constr_type == C_STR)
		return new T(string.c_str());
	return nullptr;
}