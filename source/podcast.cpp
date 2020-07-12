#include "podcast.hpp"

static Logger::Module log("podcast");

Podcast::Podcast()
{}

int Podcast::parseFromXmlStream(const std::string &xml)
{
    tinyxml2::XMLDocument stream;
    stream.Parse(xml.c_str(), xml.size());
    typedef tinyxml2::XMLElement* node_ptr; 

    node_ptr rootIndex = stream.FirstChildElement("rss")->FirstChildElement("channel");
    if(!rootIndex)
        return PARSER_CHANNEL_ERROR;
    
    node_ptr name = rootIndex->FirstChildElement("title");
    if(!name)
        return PARSER_TITLE_ERROR;

    node_ptr author = rootIndex->FirstChildElement("itunes:author");
    if(!author)
        return PARSER_AUTHOR_ERROR;

    node_ptr link = rootIndex->FirstChildElement("link");
    if(!link)
        return PARSER_LINK_ERROR;

    node_ptr desc = rootIndex->FirstChildElement("description");
    if(!desc)
        return PARSER_DESCRIPTION_ERROR;

    node_ptr img = rootIndex->FirstChildElement("itunes:image");
    if(!img)
    {
        img = rootIndex->FirstChildElement("image")->FirstChildElement("url");
        if(!img)
            return PARSER_IMAGE_ERROR;
        this->image_path = img->FindAttribute("href")->Value();
    }
    else
        this->image_path = img->FindAttribute("href")->Value();
    
    this->name = name->GetText();
    this->author = author->GetText();
    this->description = desc->GetText();
    this->url = link->GetText();

    for(node_ptr p = rootIndex->FirstChildElement("item"); p != nullptr; p = p->NextSiblingElement("item"))
    {
        if(
            p->FirstChildElement("title") != nullptr &&
            p->FirstChildElement("description") != nullptr &&
            p->FirstChildElement("enclosure") != nullptr
        )
        {
            Episode e;
            e.name = p->FirstChildElement("title")->GetText();
            e.description = p->FirstChildElement("description")->GetText();
            e.url = p->FirstChildElement("enclosure")->FindAttribute("url")->Value();
            this->episodes.push_back(e);
        }
    }

    return OK;
}