#pragma once
#include "Page.h"


class InitStringsPage : public Page
{
public:
	explicit InitStringsPage(SDL_Renderer* const renderer);

	void process_text_input(const SDL_Event& event) override;

private:
	static void back_button(const EventArguments&);
	static void confirm_button(const EventArguments& args);

	void init_bind_buttons() override;
	void check_input(const std::shared_ptr<InputBox>& input_box);

	struct InitStringsPageArgs : EventArgs
	{
		const int& amount;

		InitStringsPageArgs(const int& _amount)
			: amount(_amount){}
	};

	const int _max_amount = 6;

	int _strings_amount = 0;
};