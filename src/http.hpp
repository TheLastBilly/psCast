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

class HttpBase 
{
protected:
    static const std::string dl_dir;
    static bool is_init;

    static constexpr const char * default_dir = "ux0:/";

    static int init();
    static int end();

public:
    enum {
      OK,
      NET_MODULE_ERROR,
      NET_RUN_ERROR,
      NET_CTL_ERROR,
      NET_SOCKET_ERROR,
      HTTP_MODULE_ERROR,
      HTTP_RUN_ERROR,
      CURL_ERROR,
      NOT_INIT
    };

    HttpBase(){}

    virtual int download( std::string url, std::string path ) = 0;
    int download( std::string url )
    {
        size_t pos = std::string::npos;
        if((pos = url.rfind('/')) != std::string::npos && pos < url.size() -1)
        {
            return download(url, default_dir + url.substr(pos+1) );
        }
        return download(url, std::string(default_dir) + "downloaded_file");
    }
};

class Http: public HttpBase 
{
public:
    Http():
        HttpBase(){}
    int download( std::string url, std::string path ) override;
};

class Https: public HttpBase 
{
public:
    Https():
        HttpBase(){}
    int download( std::string url, std::string path ) override;
};