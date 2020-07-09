#pragma once

#include <string>
#include <functional>
#include <vector>

#include "gui.hpp"

#define HEADER_DEBUG

class MenuLabel 
{
protected:
    std::string label;
public:
    MenuLabel(){}
    MenuLabel(std::string label):
        label(label){}
    
    bool operator==( const std::string &label )
    {
        return 
            label.size() > this->label.size() && 
            this->label.find(label) != std::string::npos;
    }
};

class MenuEntry: public MenuLabel
{
public:
    MenuEntry(){}
    MenuEntry( std::string label, std::function<void()> callback ):
        MenuLabel(label), callback(callback) {}
    
    std::function<void()> callback = nullptr;

    void run()
    {
        if(callback)
            callback();
    }
    bool okay()
    {
        return callback != nullptr;
    }
    std::string getLabel()
    {
        return label;
    }
};

class MenuList: public MenuLabel
{
private:
    std::vector<MenuEntry> entries;

public:
    MenuList(){}

    MenuList(std::string label):
        MenuLabel(label){}
    MenuList(std::string label, std::vector<MenuEntry> entries):
        MenuLabel(label), entries(entries) {} 

    MenuList(const MenuList &ml):
        MenuLabel(ml.label)
    {
        entries = ml.entries;
    }

    std::string getLabel()
    {
        return label;
    }

    void append( const MenuEntry &entry )
    {
        entries.push_back(entry);
    }

    void erase( size_t index )
    {
        if(entries.size() > index)
            entries.erase( entries.begin() + index );
    }
    void erase( const std::string index )
    {
        for(size_t e = 0; e < entries.size(); e++)
        {
            if(entries.at(e) == index)
                entries.erase( entries.begin() + e );
        }
    }

    size_t size()
    {
        return entries.size();
    }

    MenuEntry operator[](const size_t index)
    {
        if(index < entries.size())
            return entries[index];
        
        return MenuEntry();
    }
    MenuEntry operator[](const std::string index)
    {
        for(MenuEntry entry : entries)
            if(entry == index)
                return entry;
            
        return MenuEntry();
    }
    MenuEntry at( const size_t index ) 
    {
        return (*this)[index];
    }
};

/*
##########################################
## MENU LAYOUT  (NAME: [WITDH, HEIGHT]) ##
##########################################

-------------------------------------------
BATTERY/SYSTEM INFO [960, 40]
-------------------------------------------
CURRENT CONTEXT [960, 72]
-------------------------------------------
CONTENT [960, 54]
CONTENT [960, 54]
CONTENT [960, 54]
CONTENT [960, 54]
CONTENT [960, 54]
CONTENT [960, 54]
CONTENT [960, 54]
CONTENT [960, 54]
-------------------------------------------
*/


class Menu: public GUI
{
private:
    static constexpr const uint 
        info_height = 40,
        current_context_height = 72,
        entry_count = 8;
    static constexpr const uint entry_height = 
        (PSCAST_DISPLAY_HEIGHT - (info_height + current_context_height))/entry_count;

    size_t entry_index = 0;
    MenuList *menu_list = nullptr;
    MenuList internal_menu_list;

    uint entry_width = PSCAST_DISPLAY_WIDTH;

    uint clear_text_color = RGBA8(240, 240, 230, 255);
    uint dark_text_color = RGBA8(51, 51, 51, 255);
    uint active_color = RGBA8(140, 140, 130, 255);
    
    std::string status_msg = "";
public:
    Menu();

    ~Menu();

    void minimizeList();
    void expandList();

    void checkoutEntryList( const MenuList &menu_list );
    void checkoutEntryList( MenuList *menu_list );
    bool setEntryActive( size_t index );
    bool setEntryActive( std::string index );

    void drawList( uint index );
    void drawList( uint index, uint increment );
    void drawHeader();

    void setStatusLabel( const std::string &label );

    void goDownOnList();
    void goUpOnList();

    int setup() override;
    int draw() override;
};