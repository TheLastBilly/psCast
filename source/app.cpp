#include <string>

#include "app.hpp"

static Logger::Module log("app");

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
            MenuEntry("podcasts", [this](){menu.goToList(&podcast_list);}),
            MenuEntry("add url", [this](){appendUrlToFeed();}),
            MenuEntry("update", [this](){updateFromFeedFile();}),
            MenuEntry("options", [this](){menu.goToList(&options_list);}),
        }
    ),

    menu(&main_menu_list)
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
    menu.setAcceptButton(SCE_CTRL_CIRCLE);
    menu.setBackButton(SCE_CTRL_CROSS);
    
	GUI::checkoutActiveGUI(&menu);
    podcast_list.setLabel("Podcasts");
}
void App::run()
{
	while(SCE_TRUE)
	{
		GUI::cycleActiveGUI();
	}
}

Podcast App::downloadAndParseFeed(const std::string &url)
{
    
    Podcast podcast;
    HttpBase * dl = nullptr;
    try
    {
        menu.sendHeaderMessage("Downloading: " + url + "...");
        log.log("downloading \"" + url + "\"...");
        int st;

        if(url.find("https://") != std::string::npos)
            dl = new Https();
        else
            dl = new Http();

        st = dl->download(url);

        menu.sendHeaderMessage("Parsing " + url + "...");
        log.log("started parsing for \"" + url + "\"");
        if(dl->getCurrentPage().size() < 1)
            throw std::runtime_error("request result from \"" + url + "\" is empty");
        
        st = podcast.parseFromXmlStream(dl->getCurrentPage());
    }
    catch(const std::exception &e)
    {
        if(dl != nullptr)
            delete dl;
        
        log.log("Parser error: " + std::string(e.what()));
        menu.sendHeaderMessage("Error!");
        return podcast;
    }
    
    if(dl != nullptr)
        delete dl;

    menu.sendHeaderMessage("Done!");
    return podcast;
}

int App::updateFeedFile()
{
    feed_urls = Utils::getLinesFromFile(PSCAST_RSS_LIST_FILE);
    if(feed_urls.size() < 1)
        return -1;

    return OK;
}

int App::updateFromFeedFile()
{
    if(updateFeedFile() != OK)
        return -1;

    podcasts.clear();
    for(std::string &s : feed_urls)
    {
        Podcast p = downloadAndParseFeed(s);
        if(p.isValid())
            podcasts.push_back(p);
    }
    
    updatePodcastsList(podcasts);
    return OK;
}

void App::updatePodcastsList(const std::vector<Podcast> &podcasts)
{
    for(const Podcast &p : podcasts)
    {
        podcast_list.append(
            MenuEntry(p.name, nullptr)
        );
    }
}

std::string App::requestInputFromImeDialog(std::string title, SceUInt32 type)
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
	param.type = type;
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

        if(sceImeDialogGetStatus() == SceCommonDialogStatus::SCE_COMMON_DIALOG_STATUS_FINISHED)
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

void App::appendUrlToFeed()
{
    updateFeedFile();
    std::string input = requestInputFromImeDialog("Feed URL", SCE_IME_TYPE_BASIC_LATIN);
    if(input.size() < 1)
        return;

    for(std::string &s : feed_urls)
        if(s == input)
        {
            menu.sendHeaderMessage("\"" + s + "\" already in list");
            return;
        }
    
    int fd = sceIoOpen(PSCAST_RSS_LIST_FILE, SCE_O_WRONLY | SCE_O_APPEND, 0777);
    if(fd < 1)
    {
        menu.sendHeaderMessage("cannot open \"" PSCAST_RSS_LIST_FILE  "\"");
        log.log("cannot open \"" PSCAST_RSS_LIST_FILE  "\"");
        return;
    }

    input.append("\n");
    sceIoWrite(fd, input.c_str(), input.size());
    menu.sendHeaderMessage("appended \"" + input + "\" to feed list");
    sceIoClose(fd);
}