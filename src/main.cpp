#include <iostream>
#include <cstdio>

#include <vita2d.h>

#include <psp2/appmgr.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>

#include "app.hpp"

int main(int argc, char const *argv[])
{
	App app;
	App::init();
	app.setup();
	app.run();
	App::end();
	sceKernelExitProcess(0);

	return 0;
}
