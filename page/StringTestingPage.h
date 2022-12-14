#pragma once
#include "StringTestingPageBase.h"

#include "../string/String.h"

template<class Str> requires std::derived_from<Str, BaseString>
class StringTestingPage : public StringTestingPageBase
{
public:
	explicit StringTestingPage(SDL_Renderer* const renderer);

	void process_text_input(const SDL_Event& event) override;

protected:
	void create_methods_text_boxes() override;

private:
	struct StringIndexArgs : ButtonArgs
	{
		const size_t string_index;

		StringIndexArgs(const std::shared_ptr<Button>& button, const size_t string_index)
			: ButtonArgs(button), string_index(string_index){}
	};

	static void string_button(const EventArguments& args);
	static void find_char_button(const EventArguments&);
	static void cancel_button(const EventArguments& args);

	static void operator_method_button(const EventArguments& args);

	void create_string_buttons() override;
	void init_bind_buttons() override;
};

template <class Str> requires std::derived_from<Str, BaseString>
void StringTestingPage<Str>::find_char_button(const EventArguments&)
{}