#include "podcast.hpp"

static Logger::Module log("podcast");

Podcast::Podcast()
{}

int Podcast::parseFromXmlStream(const std::string &xml)
{
    is_valid = false;

    feedpp::parser parser;
    feedpp::feed feed = parser.parse_buffer(xml.c_str(), xml.size());
        
    this->name = feed.title;
    this->author = feed.dc_creator;
    this->url = feed.link;
    this->description = feed.description;

    for(feedpp::item &item : feed.items)
    {
        Episode e;
        e.name = item.title;
        e.description = item.description;
        e.url = item.link;

        episodes.push_back(e);
    }

    is_valid = true;
    return OK;
}