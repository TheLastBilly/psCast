#pragma once

class GUI
{
private:
    static bool is_init;
public:
    enum 
    {
        OK
    };

    static int Init();
    static int End();

    bool isInit()
    {
        return is_init;
    }

    virtual int updateInput() = 0;
    virtual int draw() = 0;
};