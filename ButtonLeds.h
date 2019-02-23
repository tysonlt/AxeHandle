#pragma once

class ButtonLeds {

  public:

    void on(byte number);
    void off(byte number);
    void dim(byte number, byte brightness);
    void flash(byte number, unsigned long interval = 100, byte repeat = 1);

    void clear();
    void update();
    
};