#pragma once

#include <psp2/appmgr.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>
#include <psp2/ime_dialog.h>

#include <memory>

#include "logger.hpp"
#include "menu.hpp"
#include "settings.hpp"
#include "podcast.hpp"
#include "http.hpp"

#include "utils.hpp"

class App
{
public:
    enum 
    {
        OK
    };

    App();
    ~App();

    static void init();
    static void end();

    void setup();
    void run();

    void goToList(MenuList *next_list);
    void goToList(MenuList *next_list, MenuList * previous_list);
    
    Podcast downloadAndParseFeed(const std::string &url);
    int updateFeedFile();
    int updateFromFeedFile();
    void updatePodcastsList(const std::vector<Podcast> &podcasts);

    std::string requestInputFromImeDialog(std::string title, SceUInt32 type);

    void appendUrlToFeed();

    std::vector<Podcast> podcasts;
private:
    Menu menu;

    MenuList main_menu_list;
    MenuList options_list;
    MenuList podcast_list;

    std::vector<std::string> feed_urls;
    std::vector<MenuList *> podcast_episodes_lists;

    enum WINDOW
    {
        OPTIONS,
        MAIN_MENU,
    };
};