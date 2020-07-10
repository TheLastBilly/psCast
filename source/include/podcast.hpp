#pragma once

#include <string>
#include <vector>

#include "logger.hpp"
#include "tinyxml2.h"

class Podcast
{
public:
    enum
    {
        OK,
        PARSER_CHANNEL_ERROR,
        PARSER_TITLE_ERROR,
        PARSER_AUTHOR_ERROR,
        PARSER_LINK_ERROR,
        PARSER_DESCRIPTION_ERROR,
        PARSER_IMAGE_ERROR,
    };

    std::string name; 
    std::string author;
    std::string description;

    std::string image_path;
    std::string url;

    struct Episode
    {
        std::string name;
        std::string description;

        std::string url;
        unsigned long duration;

        bool alreadyPlayed = false;
        uint32_t id;

        bool init;
    };

    std::vector<Episode> episodes;

    Podcast();
    Podcast(const Podcast &podcast)
    {
        name = podcast.name;
        author = podcast.author;
        description = podcast.description;

        image_path = podcast.image_path;
        url = podcast.url;

        episodes = podcast.episodes;
    }

    int parseFromXmlStream(const std::string &xml);
};