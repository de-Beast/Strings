#include "MainMenuPage.h"
#include "InfoPage.h"
#include "InitStringsPage.h"
#include "StringMenuPage.h"

#include "../Globals.h"

#include "../UI/Button.h"
#include "../string/StringDataBase.h"

MainMenuPage::MainMenuPage(SDL_Renderer* renderer)
	: Page(renderer)
{
	Dimensions dimens(300, 50);
	SDL_Point coords((WINDOW_WIDTH - dimens.w) / 2,
					 2 * WINDOW_HEIGHT / 6 - dimens.h);
	create_object<Button>("StartButton", dimens, coords, "Запустить");

	coords = SDL_Point((WINDOW_WIDTH - dimens.w) / 2,
					   3 * WINDOW_HEIGHT / 6 - dimens.h);
	create_object<Button>("InfoButton", dimens, coords, "Справка");

	coords = SDL_Point((WINDOW_WIDTH - dimens.w) / 2,
					   4 * WINDOW_HEIGHT / 6 - dimens.h);
	create_object<Button>("QuitButton", dimens, coords, "Выход");

	MainMenuPage::init_bind_buttons();
}

void MainMenuPage::start_button(const EventArguments&)
{
	try
	{
		StringDataBase::get_instance();
		init_page<StringMenuPage>(get_page<MainMenuPage>()->_renderer);
	}
	catch (const std::runtime_error&)
	{
		init_page<InitStringsPage>(get_page<MainMenuPage>()->_renderer);
	}
}

void MainMenuPage::info_button(const EventArguments&)
{
	init_page<InfoPage>(get_page<MainMenuPage>()->_renderer);
}

void MainMenuPage::quit_button(const EventArguments&)
{
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}


void MainMenuPage::init_bind_buttons()
{
	std::shared_ptr<Button> button;

	if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("StartButton"))) != nullptr)
		button->bind_on_click(&start_button);

	if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("InfoButton"))) != nullptr)
		button->bind_on_click(&info_button);

	if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("QuitButton"))) != nullptr)
		button->bind_on_click(&quit_button);
}
