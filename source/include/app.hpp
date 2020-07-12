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

    static void init();
    static void end();

    void setup();
    void run();

    void goToWindow(MenuList *next_list);
    void goToWindow(MenuList *next_list, MenuList * previous_list);
    
    Podcast downloadAndParseFeed(const std::string &url);
    int updateFromFeedFile();

    std::string requestInputFromImeDialog(std::string title);
private:
    Menu menu;

    MenuList main_menu_list;
    MenuList options_list;

    enum WINDOW
    {
        OPTIONS,
        MAIN_MENU,
    };
};