#include "utils.hpp"

static Logger::Module log("utils");

//Based on utf8_to_utf16 from ScoreHax.c on line 58, written by KuromeSan
//https://github.com/KuromeSan/ScoreHax/blob/master/ScoreHax.c
void Utils::utf8To16(const uint8_t *src, size_t src_size, uint16_t *dst, size_t dst_size) 
{
    if(dst_size < src_size*2)
        return;

    for(size_t i = 0; i < src_size && src[i]; i++)
    {
        if((src[i] & 0xe0) == 0xe0 && src_size - i > 2)
        {
            *(dst++) = ((src[i] & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
            i += 2;
        }
        else if((src[i] & 0xC0) == 0xC0 && src_size - i > 1)
        {
            *(dst++) = ((src[i] & 0x1F) << 6) | (src[i + 1] & 0x3F);
            i++;
        }
        else
            *(dst++) = src[i];
    }

    *dst = '\0';
}

//Based on utf16_to_utf8 from ScoreHax.c on line 34, written by KuromeSan
//https://github.com/KuromeSan/ScoreHax/blob/master/ScoreHax.c
void Utils::utf16to8(const uint16_t *src, size_t src_size, uint8_t *dst, size_t dst_size)
{
    if(dst_size < src_size/2)
        return;
    
	for (size_t i = 0; i < src_size && src[i]; i++) {
		if ((src[i] & 0xFF80) == 0) {
			*(dst++) = src[i] & 0xFF;
		} else if((src[i] & 0xF800) == 0) {
			*(dst++) = ((src[i] >> 6) & 0xFF) | 0xC0;
			*(dst++) = (src[i] & 0x3F) | 0x80;
		} else if((src[i] & 0xFC00) == 0xD800 && (src[i + 1] & 0xFC00) == 0xDC00) {
			*(dst++) = (((src[i] + 64) >> 8) & 0x3) | 0xF0;
			*(dst++) = (((src[i] >> 2) + 16) & 0x3F) | 0x80;
			*(dst++) = ((src[i] >> 4) & 0x30) | 0x80 | ((src[i + 1] << 2) & 0xF);
			*(dst++) = (src[i + 1] & 0x3F) | 0x80;
			i += 1;
		} else {
			*(dst++) = ((src[i] >> 12) & 0xF) | 0xE0;
			*(dst++) = ((src[i] >> 6) & 0x3F) | 0x80;
			*(dst++) = (src[i] & 0x3F) | 0x80;
		}
	}

	*dst = '\0';
}

std::vector<std::string> Utils::getLinesFromFile(const char * file_name)
{
    std::vector<std::string> lines;

    int fd = sceIoOpen(file_name, SCE_O_RDONLY | SCE_O_CREAT, 0777);
    if(fd < 1)
    {
        log.log("Cannot open \"" + std::string(file_name) + "\n");
        return lines;
    }
    
    size_t fs = sceIoLseek(fd, 0, SCE_SEEK_END);
    sceIoLseek(fd, 0, SCE_SEEK_SET);
    
    if(fs < 1)
    {
        sceIoClose(fd);
        return lines;
    }

    char *buffer = (char *)malloc(fs*sizeof(char));
    sceIoRead(fd, buffer, fs);
    sceIoClose(fd);

    size_t index = 0, past_index = 0;
    while( index < fs )
    {
        if(buffer[index] == '\n')
        {
            lines.push_back(std::string(&buffer[past_index], index - past_index));
            past_index = index+1;
        }
        index++;
    }
    
    free(buffer);
    return lines;
}