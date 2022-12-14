#include "InfoPage.h"
#include "MainMenuPage.h"

#include "../Globals.h"

#include "../UI/Button.h"
#include "../UI/TextBox.h"

InfoPage::InfoPage(SDL_Renderer* const renderer)
	: Page(renderer)
{
	Dimensions dims{600, 500};
	create_object<TextBox>("InfoBox", dims, { (WINDOW_WIDTH - dims.w) / 2, (WINDOW_HEIGHT - dims.h) / 2}, "Данное приложение предназначено для работы со строками, приятного использования!!!\nСоздатель Литвинов Д.С.\nгруппа О716Б");

	dims = {120, 30};
	create_object<Button>("BackButton", dims, SDL_Point(30, WINDOW_HEIGHT - 2 * dims.h), "Назад", 30);

	InfoPage::init_bind_buttons();
}

void InfoPage::back_button(const EventArguments&)
{
	init_page<MainMenuPage>(get_page<InfoPage>()->_renderer);
}

void InfoPage::init_bind_buttons()
{
	std::shared_ptr<Button> button;

	if ((button = std::dynamic_pointer_cast<Button>(_ui_elems.at("BackButton"))) != nullptr)
		button->bind_on_click(&back_button);
}