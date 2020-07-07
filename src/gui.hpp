#pragma once

#include <psp2/ctrl.h>
#include <psp2/types.h>
#include <vita2d.h>

#include <cstring>

#define PSCAST_DISPLAY_WIDTH 960
#define PSCAST_DISPLAY_HEIGHT 544

class GUI
{
public:
    enum 
    {
        OK,
        BAD_PARAMETER,
    };

protected:
    SceCtrlData ctrl_data;
    SceUInt32 pressed_buttons = 0;
    static constexpr const size_t 
        input_delay = 500;

    virtual int setup() = 0;
    virtual int draw() = 0;
    virtual void updateInput( SceCtrlData &ctrl_data );

    virtual void update()
    {
        memset(&ctrl_data, 0, sizeof(ctrl_data));
        sceCtrlPeekBufferNegative(0, &ctrl_data, 1);

        updateInput(ctrl_data);
        draw();
    }

    static vita2d_font * system_font;
    
private:
    static GUI * current_gui;
    static bool is_init;

public:
    GUI(){}

    static void init()
    {
        if(is_init)
            return;
        
    	vita2d_init();

        system_font = 
            vita2d_load_font_file("app0:system_font.tff");
        is_init = true;
    }
    static void end()
    {
        if(!is_init)
            return;
        
    	vita2d_fini();
        is_init = false;
    }   

    static int checkoutActiveGUI( GUI * gui )
    {
        if(gui != nullptr)
        {
            current_gui = gui;
            current_gui->setup();
            return OK;
        }
        return BAD_PARAMETER;
    }
    static void cycleActiveGUI()
    {
        if(current_gui != nullptr)
            current_gui->update();
    }
};