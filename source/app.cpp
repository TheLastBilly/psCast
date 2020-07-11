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
            MenuEntry("add url", [this](){menu.setStatusLabel(requestInputFromImeDialog("lo hice bitch"));}),
            MenuEntry("update", [this](){updateFromFeedFile();}),
            MenuEntry("options", [this](){goToWindow(&options_list);}),
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
    menu.setAcceptButton(SCE_CTRL_CIRCLE);
    menu.setBackButton(SCE_CTRL_CROSS);
    
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

std::string App::requestInputFromImeDialog(std::string title)
{
    SceCommonDialogConfigParam dparam;
    sceCommonDialogConfigParamInit(&dparam);
    sceCommonDialogSetConfigParam((const SceCommonDialogConfigParam *)&dparam);

    SceImeDialogParam param;
    sceImeDialogParamInit(&param);

    SceWChar16 imeInputBuffer[SCE_IME_DIALOG_MAX_TEXT_LENGTH] = {0};

    SceWChar16 titleBuffer[title.size()] = {0};
    Utils::utf8To16((const uint8_t *)title.c_str(), title.size(), (uint16_t *)titleBuffer, sizeof(titleBuffer));

    param.sdkVersion = 0x03150021,
	param.supportedLanguages = 0x0001FFFF;
	param.languagesForced = SCE_TRUE;
	param.type = SCE_IME_TYPE_DEFAULT;
	param.title = titleBuffer;
	param.maxTextLength = 20;
	param.inputTextBuffer = imeInputBuffer;

    char buffer[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
    bool showDial = true;
    while(true)
    {
        if(showDial)
        {
            sceImeDialogInit(&param);
            showDial = false;
        }

        if(sceImeDialogGetStatus() == 2)
        {
            SceImeDialogResult rst;
            memset(&rst, 0, sizeof(SceImeDialogResult));

            if(rst.button == SCE_IME_DIALOG_BUTTON_CLOSE)
            {
                break;
            }
            else 
            {
                Utils::utf16to8(
                    (const uint16_t *)imeInputBuffer, SCE_IME_DIALOG_MAX_TEXT_LENGTH, 
                    (uint8_t *)buffer, SCE_IME_DIALOG_MAX_TEXT_LENGTH
                );
                break;
            }
        }

        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
    sceImeDialogTerm();

    return std::string(buffer);
}