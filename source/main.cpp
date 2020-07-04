#include <iostream>
#include <cstdio>

#include <vita2d.h>

#include <psp2/appmgr.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>

int main(int argc, char const *argv[])
{
	vita2d_init();
	while(SCE_TRUE)
	{
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_rectangle(0, 0, 960, 40, RGBA8(40, 40, 40, 255));
		vita2d_draw_rectangle(0, 40, 960, 72, RGBA8(51, 51, 51, 255));

		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	vita2d_fini();
	sceKernelExitProcess(0);

	return 0;
}
