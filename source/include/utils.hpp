#pragma once

#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/sysmodule.h>
#include <psp2/shellutil.h>

#include <cstdlib>
#include <cstdint>

#include <vector>
#include <string>

#include "logger.hpp"

class Utils
{
public:
    static void utf8To16(const uint8_t *src, size_t src_size, uint16_t *dst, size_t dst_size);
    static void utf16to8(const uint16_t *src, size_t src_size, uint8_t *dst, size_t dst_size);

    static std::vector<std::string> getLinesFromFile(const char * file_name);
};