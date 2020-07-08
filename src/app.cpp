#include "app.hpp"

App::App():
    options_list(
        "options",
        {
            MenuEntry("test", nullptr),
            MenuEntry("back", [=](){goToWindow(WINDOW::MAIN_MENU);})
        }
    ),

    main_menu_list(
        "psCast",
        {
            MenuEntry("add from rss link", [=](){return;}),
            MenuEntry("show podcasts", [=](){return;}),
            MenuEntry("options", [=](){goToWindow(WINDOW::OPTIONS);}),
        }
    )
{}

void App::init()
{
	SceAppUtilInitParam init;
	SceAppUtilBootParam boot;

	memset(&init, 0, sizeof(SceAppUtilInitParam));
	memset(&boot, 0, sizeof(SceAppUtilBootParam));
	
	sceAppUtilInit(&init, &boot);
	sceAppUtilMusicMount();

	GUI::init();

    vita2d_set_clear_color(RGBA8(250, 250, 250, 255));
}
void App::end()
{
	GUI::end();
}

void App::setup()
{
    menu.checkoutEntryList(&main_menu_list);
	GUI::checkoutActiveGUI(&menu);
}
void App::run()
{
	while(SCE_TRUE)
	{
		GUI::cycleActiveGUI();
	}
}

void App::goToWindow(App::WINDOW window)
{
    switch (window)
    {
    case OPTIONS:
        menu.checkoutEntryList(&options_list);
        break;
    case MAIN_MENU:
        menu.checkoutEntryList(&main_menu_list);
        break;
    }
}