#include <iostream>
#include <cstdio>

#include <vita2d.h>

#include <psp2/appmgr.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>

#include "http.hpp"
#include "menu.hpp"

int main(int argc, char const *argv[])
{
	SceAppUtilInitParam init;
	SceAppUtilBootParam boot;
	memset(&init, 0, sizeof(SceAppUtilInitParam));
	memset(&boot, 0, sizeof(SceAppUtilBootParam));
	
	int ret = 0;
	
	sceAppUtilInit(&init, &boot);

	sceAppUtilMusicMount();
	
	GUI::init();
	vita2d_set_clear_color(RGBA8(250, 250, 250, 255));
	Menu *menu = new Menu();
	MenuEntry entry("Test", nullptr);
	MenuList list;
	for(int i = 0; i < 15; i++)
		list.append( entry );
	menu->checkoutEntryList(list);
	GUI::checkoutActiveGUI(menu);
	while(SCE_TRUE)
	{
		GUI::cycleActiveGUI();
	}
	delete menu;
	GUI::end();
	sceKernelExitProcess(0);

	return 0;
}
