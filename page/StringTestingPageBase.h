#pragma once

#include "SDL.h"
#include "Page.h"
#include "../string/StringDataBase.h"

class StringTestingPageBase : public Page
{
public:
	explicit StringTestingPageBase(SDL_Renderer* const renderer);

protected:
	struct StringMethodArgs : ButtonArgs
	{
		const bool is_find_char;

		StringMethodArgs(const std::shared_ptr<Button>& button, const bool is_find_char)
			: ButtonArgs(button), is_find_char(is_find_char){}
	};

	struct TestMethodArgs : EventArgs
	{
		const StringDataBase::ETestOperator test_operator;
		const int cancel_button_index;

		TestMethodArgs(const StringDataBase::ETestOperator test_method, const int cancel_button_index)
			: test_operator(test_method), cancel_button_index(cancel_button_index){}
	};

	static void cancel_button(const EventArguments& args);
	static void operator_method_button(const EventArguments& args);

	static void show_string_buttons(const bool do_show);

	void create_string_text_box();
	void erase_oparators_results();
	virtual void create_methods_text_boxes();
	void init_bind_buttons() override;

	inline static StringDataBase* _db;

	int _strings_amount = 0;

private:
	static void back_button(const EventArguments&);

	virtual void create_string_buttons() = 0;
};
