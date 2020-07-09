#include "podcast.hpp"

Podcast::Podcast()
{}

int Podcast::parseFromXmlStream(const std::string &xml)
{
    typedef rapidxml::xml_node<>* node_ptr;

    rapidxml::xml_document<> feed;
    feed.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>((char *)xml.c_str());

    node_ptr rootIndex = feed.first_node("rss")->first_node("channel");
    if(!rootIndex)
        return PARSER_CHANNEL_ERROR;

    node_ptr name = rootIndex->first_node("title");
    if(!name)
        return PARSER_TITLE_ERROR;

    node_ptr author = rootIndex->first_node("itunes:author");
    if(!author)
        return PARSER_AUTHOR_ERROR;

    node_ptr link = rootIndex->first_node("link");
    if(!link)
        return PARSER_LINK_ERROR;

    node_ptr desc = rootIndex->first_node("description");
    if(!desc)
        return PARSER_DESCRIPTION_ERROR;

    node_ptr img = rootIndex->first_node("itunes:image");
    if(!img)
    {
        img = rootIndex->first_node("image")->first_node("url");
        if(!img)
            return PARSER_IMAGE_ERROR;
        this->image_path = img->first_attribute("href")->value();
    }
    else
        this->image_path = img->first_attribute("href")->value();
    
    this->name = name->value();
    this->author = author->value();
    this->description = desc->value();
    this->url = link->value();

    for(node_ptr p = rootIndex->first_node("item"); p != nullptr; p->next_sibling())
    {
        if(
            p->first_node("title") != nullptr &&
            p->first_node("description") != nullptr &&
            p->first_node("enclosure") != nullptr
        )
        {
            Episode e;
            e.name = p->first_node("title")->value();
            e.description = p->first_node("description")->value();
            e.url = p->first_node("enclosure")->first_attribute("url")->value();
            this->episodes.push_back(e);
        }
    }

    return true;
}