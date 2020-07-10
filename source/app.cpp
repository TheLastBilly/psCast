#include "app.hpp"
#include "podcast.hpp"
#include "http.hpp"

App::App():
    options_list(
        "options",
        {
            MenuEntry("test", [=](){
                Https https;
                int st = https.download("https://locono.live/feed.rss");
                Podcast podcast;
                try
                {
                    st = podcast.parseFromXmlStream(https.getCurrentPage());
                }
                catch(const std::exception &e)
                {
                    Logger::nlog("Parser error: " + std::string(e.what()));
                }
                menu.setStatusLabel("PARSER: " + std::to_string(st));
                Logger::nlog(std::string("name: ") + podcast.name);
                Logger::nlog(std::string("description: ") + podcast.description);
                Logger::nlog(std::string("url: ") + podcast.url);
                Logger::nlog(std::string("author: ") + podcast.author);

                for(Podcast::Episode e : podcast.episodes)
                {
                    Logger::nlog(std::string("name: ") + e.name);
                    Logger::nlog(std::string("description: ") + e.description);
                    Logger::nlog(std::string("url: ") + e.url);
                }
            }),
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
	struct SceIoStat * dirStat = (SceIoStat*)malloc(sizeof(SceIoStat));
	if(sceIoGetstat(PSCAST_DATA_FOLDER , dirStat) < 0){
		sceIoMkdir(PSCAST_DATA_FOLDER , 0777);
	}
    free(dirStat);

    Logger::init(PSCAST_DATA_FOLDER "syslog.txt");

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