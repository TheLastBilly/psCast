#include "logger.hpp"

#include <psp2/io/fcntl.h>

bool Logger::is_init = false;
int Logger::fd = 0;

void Logger::writeToFile(const std::string &msg)
{
    if(!is_init || fd < 1)
        return;

    sceIoWrite(fd, msg.c_str(), msg.size());
}

int Logger::init(const char * log_file)
{
    fd = sceIoOpen(log_file, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0777);
    if(fd < 1)
        return FILE_OPEN_ERROR;
    
    is_init = true;
    return OK;
}

void Logger::end()
{
    if(fd > 0)
        sceIoClose(fd);   

    fd = 0;
    is_init = false;
}