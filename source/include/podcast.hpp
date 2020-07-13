#pragma once

#include <string>
#include <vector>

#include "logger.hpp"
#include "feedpp.h"

class Podcast
{
private:
    bool is_valid = false;
public:
    enum
    {
        OK
    };

    std::string name; 
    std::string author;
    std::string description;

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
        url = podcast.url;

        episodes = podcast.episodes;
    }

    int parseFromXmlStream(const std::string &xml);
    
    bool isValid()
    {return is_valid;}
};