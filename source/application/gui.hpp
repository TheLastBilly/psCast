#pragma once

class GUI
{
public:
    enum 
    {
        OK
    };

    static int Init();
    static int End();

    virtual int draw() = 0;
    virtual int updateInput() = 0;
};