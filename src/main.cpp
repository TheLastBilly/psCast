#include <iostream>
#include <cstdio>

#include <vita2d.h>

#include <psp2/appmgr.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>

#include "http.hpp"

int main(int argc, char const *argv[])
{
	HttpBase::Init();
	HttpBase * dl = new Https();
	dl->download("https://www.example.com");
	delete dl;
	HttpBase::End();
	sceKernelExitProcess(0);

	return 0;
}
