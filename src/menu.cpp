#include "menu.hpp"

Menu::Menu()
{}

Menu::~Menu()
{
    if(menu_list != nullptr)
        delete menu_list;
}

void Menu::checkoutEntryList( const MenuList &menu_list )
{
    if(this->menu_list != nullptr)
        delete this->menu_list;
    
    this->menu_list = new MenuList(menu_list);
    entry_index = 0;
}

bool Menu::setEntryActive( size_t index )
{
    if(menu_list->size() > index)
    {
        entry_index = index;
        return true;
    }
    return false;
}

bool Menu::setEntryActive( std::string index )
{
    for(size_t e = 0; e < menu_list->size(); e++)
    {
        if(menu_list->at(e) == index)
            entry_index = e;
    }
}

void Menu::drawList( uint index )
{   
    if(menu_list == nullptr)
        return;
    
    size_t increment = entry_count*(index/8);

    size_t m_s = menu_list->size();
    uint i = increment*entry_count;
    if(i < m_s && m_s < 1)
        return;
    
    int start = info_height + current_context_height;
    uint color = 0;

    while(i < m_s && start < PSCAST_DISPLAY_HEIGHT)
    {
        if(index == i)
            color = active_color;
        else
            color = released_color;
        
        vita2d_draw_rectangle(0, start, entry_width, entry_height, color);
        vita2d_font_draw_text(system_font, 10, start + entry_height*0.7, RGBA8(51, 51, 51, 255), entry_height/2, menu_list->at(i).getLabel().c_str());
        start+=entry_height;
        i++;
    }
}

void Menu::drawHeader()
{
    vita2d_draw_rectangle(0, 0, PSCAST_DISPLAY_WIDTH, info_height, RGBA8(40, 40, 40, 255));
    vita2d_draw_rectangle(0, info_height, PSCAST_DISPLAY_WIDTH, current_context_height, RGBA8(51, 51, 51, 255));
}

void Menu::goDownOnList()
{
    if(menu_list == nullptr &&  entry_index > 0)
        return;
    
    if(entry_index < menu_list->size() - 1)
        entry_index++;
}
void Menu::goUpOnList()
{
    if(menu_list == nullptr)
        return;
    
    if(entry_index > 0)
        entry_index--;
}

int Menu::setup()
{
    return OK;
}

int Menu::draw()
{
    switch (pressed_buttons)
    {
    case SCE_CTRL_DOWN:
        goDownOnList();
        break;
    case SCE_CTRL_UP:
        goUpOnList();
        break;
    default:
        break;
    }

    vita2d_start_drawing();
    drawHeader();
    drawList(entry_index);
    vita2d_end_drawing();
    vita2d_swap_buffers();

    return OK;
}