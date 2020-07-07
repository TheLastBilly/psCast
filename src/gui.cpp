#include "gui.hpp"

GUI * GUI::current_gui = nullptr;
bool GUI::is_init = false;
vita2d_font * GUI::system_font = nullptr;

void GUI::updateInput( SceCtrlData &ctrl_data )
{
    static SceUInt32 old_data = 0xffffffff;
    
    pressed_buttons = 
        ctrl_data.buttons & ~old_data;
}