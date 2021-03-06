#pragma once

#include <psp2/io/fcntl.h>
#include <psp2/rtc.h>

#include <ctime>
#include <iostream>
#include <sstream>

class Logger
{
private:
    static bool is_init;
    static int fd;

    static void writeToFile(const std::string &msg);
public:
    enum
    {
        FILE_OPEN_ERROR,
        OK
    };

    static int init(const char *log_file);
    static void end();

    static void log(const std::string &msg)
    {
        writeToFile(msg);
    }
    static void nlog(const std::string &msg)
    {
        writeToFile(msg + "\n");
    }

    class Module
    {
    private:
        std::string name;
    public:
        Module(const std::string &name):
            name(name)
        {}

        void log(const std::string &msg)
        {Logger::nlog(name + ": " + msg);}
    };
};