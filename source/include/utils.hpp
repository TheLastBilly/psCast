#pragma once

#include <cstdlib>
#include <cstdint>

class Utils
{
public:
    static void utf8To16(const uint8_t *src, size_t src_size, uint16_t *dst, size_t dst_size);
    static void utf16to8(const uint16_t *src, size_t src_size, uint8_t *dst, size_t dst_size);
};