#include <string>

#include "app.hpp"

App::App():
    options_list(
        "options",
        {
            MenuEntry("test", nullptr),
        }
    ),

    main_menu_list(
        "psCast",
        {
            MenuEntry("update", [=](){updateFromFeedFile();}),
            MenuEntry("options", [=](){goToWindow(&options_list);}),
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
    Logger::log("Log started");

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

void App::goToWindow(MenuList *ml)
{
    if(ml == nullptr)
        return;
    
    if(this->current_list != nullptr)
    {
        MenuList *l = this->current_list;
        menu.setBackButtonCallback(
            [=](){
                menu.checkoutEntryList(l);
            }
        );
    }
    else
        menu.setBackButtonCallback(nullptr);
    
    this->current_list = ml;
}

Podcast App::downloadAndParseFeed(const std::string &url)
{
    Https https;
    int st = https.download(url);
    menu.setStatusLabel("Downloading: " + url);
    menu.forceHeaderDraw();
    
    Podcast podcast;
    try
    {
        st = podcast.parseFromXmlStream(https.getCurrentPage());
    }
    catch(const std::exception &e)
    {
        Logger::nlog("Parser error: " + std::string(e.what()));
    }

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

    return podcast;
}

int App::updateFromFeedFile()
{
    int fd = sceIoOpen(PSCAST_RSS_LIST_FILE, SCE_O_RDONLY | SCE_O_CREAT, 0777);
    if(fd < 1)
        return -1;
    
    size_t fs = sceIoLseek(fd, 0, SCE_SEEK_END);
    sceIoLseek(fd, 0, SCE_SEEK_SET);
    
    if(fs < 6)
        return -1;

    char *buffer = (char *)malloc(fs*sizeof(char));
    sceIoRead(fd, buffer, fs);

    size_t index = 0, past_index = 0;
    while( index < fs )
    {
        if(buffer[index] == '\n')
        {
            downloadAndParseFeed(std::string(&buffer[past_index], index-1));
            past_index = index+1;
        }
        index++;
    }

    free(buffer);
    return OK;
}