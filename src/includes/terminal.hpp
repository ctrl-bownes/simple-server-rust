#ifndef TERMINAL_HPP
#define TERMINAL_HPP

enum class Key
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    ESCAPE,
    BACKSPACE,
    CHAR,
    CTRL_C
};

struct KeyEvent
{
    Key key;
    char character;
};

void terminal_raw_mode();
void terminal_restore();
KeyEvent terminal_read_key();

#endif
