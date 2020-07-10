#pragma once

#include <string>
#include <unordered_map>

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
    std::string current_page;

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

    virtual int download( std::string url ) = 0;
    std::string getCurrentPage();
    static std::string getStatusDesc(int desc);
};

class Http: public HttpBase 
{
public:
    Http():
        HttpBase(){}
    int download( std::string url ) override;
};

class Https: public HttpBase 
{
private:
    CURLcode curl_error = CURLE_OK;
public:
    Https():
        HttpBase(){}
    int download( std::string url ) override;
    CURLcode getCurlError();
    std::string getCurlErrorDesc();
};