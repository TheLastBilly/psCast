#pragma once

#include <string>
#include <functional>
#include <vector>

#include "gui.hpp"

class MenuLabel 
{
protected:
    std::string label;
public:
    MenuLabel(std::string label):
        label(label){}
    
    bool operator==( std::string label )
    {
        return label.size() > this->label.size() && this->label.find(label) != std::string::npos;
    }
};

class MenuEntry: public MenuLabel
{
public:
    MenuEntry();
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
};

class MenuList: public MenuLabel
{
private:
    std::vector<MenuEntry> entries;

public:
    MenuList();
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
    void append( const MenuEntry entry )
    {
        entries.push_back(entry);
    }

    MenuEntry operator[](const size_t i)
    {
        if(i < entries.size())
            return entries[i];
        
        return MenuEntry();
    }
    MenuEntry operator[](const std::string i)
    {
        for(MenuEntry entry : entries)
            if(entry == i)
                return entry;
            
        return MenuEntry();
    }
};

class Menu: public GUI
{
public:
    Menu();
    int setCurrentEntryList(  );
    int setEntryListActive( int index );
    int setEntryListActive( std::string index );
};