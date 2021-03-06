#include "menu.hpp"

Menu::Menu(MenuList * MainList)
{goToList(MainList);}

Menu::~Menu()
{
    if(menu_list != nullptr)
        delete menu_list;
}

void Menu::checkoutEntryList( const MenuList &menu_list )
{    
    internal_menu_list = menu_list;
    checkoutEntryList(&internal_menu_list);
}

void Menu::checkoutEntryList( MenuList *menu_list )
{
    this->menu_list = menu_list;
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
        {
            entry_index = e;
            return true;
        }
    }

    return false;
}

void Menu::drawList( uint index )
{   
    if(menu_list == nullptr)
        return;
    
    size_t increment = index/entry_count;

    size_t m_s = menu_list->size();
    uint i = increment*entry_count;
    if(i < m_s && m_s < 1)
        return;
    
    int start = info_height + current_context_height;

    while(i < m_s && start < PSCAST_DISPLAY_HEIGHT)
    {
        vita2d_clear_screen();
        
        if(index == i)    
            vita2d_draw_rectangle(
                0, start, 
                entry_width, entry_height, 
                active_color
            );
        
        vita2d_font_draw_text(
            system_font, 10, 
            start + entry_height*0.7, dark_text_color, 
            entry_height/2, 
            menu_list->at(i).getLabel().c_str()
        );

        start+=entry_height;
        i++;
    }
}

void Menu::drawHeader()
{
    //Draw top-bar
    vita2d_draw_rectangle(
        0, 0, 
        PSCAST_DISPLAY_WIDTH, info_height, 
        RGBA8(40, 40, 40, 255)
    );
    
    vita2d_font_draw_text(
        system_font, 
        10, info_height*0.7, 
        clear_text_color, 
        info_height/2, 
        status_msg.c_str()
    );

    //Draw context
    vita2d_draw_rectangle(
        0, info_height, 
        PSCAST_DISPLAY_WIDTH, current_context_height, 
        RGBA8(51, 51, 51, 255)
    );
    vita2d_font_draw_text(
        system_font, 
        10, info_height + current_context_height*0.7, 
        clear_text_color, 
        current_context_height/2, 
        menu_list->getLabel().c_str()
    );
}

void Menu::setStatusLabel( const std::string &label )
{
    status_msg = label;
}
void Menu::forceHeaderDraw()
{
    vita2d_start_drawing();
    drawHeader();
    vita2d_end_drawing();
    vita2d_swap_buffers();
}
void Menu::sendHeaderMessage( const std::string &label )
{
    setStatusLabel(label);
    forceHeaderDraw();
}

void Menu::goDownOnList()
{   
    menu_list->increaseIndex();
}
void Menu::goUpOnList()
{
    menu_list->decreseIndex();
}

void Menu::goToList(MenuList *next_list)
{
    if(next_list == nullptr)
        return;
    
    if(next_list->getParentList() == nullptr)
        next_list->setParentList(this->current_list);
    
    setBackButtonCallback(
        [=]()
        {
            if(next_list->getParentList() == nullptr)
                return;
            
            checkoutEntryList(next_list->getParentList());
            this->current_list = next_list->getParentList();
            next_list->setParentList(nullptr);
        }
    );

    this->current_list = next_list;
    checkoutEntryList(next_list);
}

int Menu::setup()
{
    return OK;
}

int Menu::draw()
{
    size_t index = 0;
    if(this->menu_list != nullptr)
    {
        if(pressed_buttons & SCE_CTRL_DOWN)
            goDownOnList();
        else if(pressed_buttons & SCE_CTRL_UP)
            goUpOnList();
        
        index = menu_list->getIndex();

        if(pressed_buttons & accept_button)
            menu_list->at(index).run();
        else if(pressed_buttons & back_button)
        {
            if(backButtonCallback != nullptr)
                backButtonCallback();
        }
        else if(pressed_buttons & options_button)
        {
            if(optionsButtonCallback != nullptr)
                optionsButtonCallback();
        }
        else if(pressed_buttons & select_button)
        {
            if(selectButtonCallback != nullptr)
                selectButtonCallback();
        }
    }

    vita2d_start_drawing();
    vita2d_clear_screen();
    drawHeader();
    drawList(index);
    vita2d_end_drawing();
    vita2d_swap_buffers();

    return OK;
}