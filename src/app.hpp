#pragma once


#include <psp2/appmgr.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>

#include <memory>
#include "menu.hpp"

class App
{
public:
    App();

    static void init();
    static void end();

    void setup();
    void run();

private:
    Menu menu;

    MenuList main_menu_list;
    MenuList options_list;

    enum WINDOW
    {
        OPTIONS,
        MAIN_MENU,
    };

    void goToWindow(WINDOW window);

};