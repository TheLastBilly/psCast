#pragma once

#include <string>

#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/display.h>

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>

#include <psp2/io/fcntl.h>

#include <curl/curl.h>

class downloader
{
private:
    enum {
      OK,
      NET_MODULE_ERROR,
      NET_RUN_ERROR,
      NET_CTL_ERROR,
      HTTP_MODULE_ERROR,
      HTTP_RUN_ERROR,
    };

    static constexpr char * default_dir = "ux0:/";

public:
    static int init();
    static int end();

    static int download( std::string url, std::string path );
    static int download( std::string url );

private:
    static const std::string dl_dir;
    static bool is_init;
};