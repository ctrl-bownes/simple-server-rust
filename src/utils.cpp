#include "includes/utils.hpp"
#include "includes/tui.hpp"

#include <iostream>

template <class T>
int numDigits(T number)
{
    int digits = 0;
    while (number) {
        number /= 10;
        digits++;
    }
    if (digits == 0)
        return (1);
    return digits;
}

int server_menu_row(int &selected)
{
    int row = selected;

    if (selected >= 3)
        row++;

    if (selected >= 7)
        row++;

    return row;
}

void edit_value(int &value, int max_input, int offset_item_y, int &selected)
{
    tui_move(OFFSET_Y + offset_item_y, OFFSET_X + numDigits(value));
    tui_show_cursor();
    std::cout << std::flush;
    tui_input_int(value, max_input);
    selected++;
}

void edit_value(std::string &value, int max_input, int offset_item_y, int &selected)
{
    tui_move(OFFSET_Y + offset_item_y, OFFSET_X + value.length());
    tui_show_cursor();
    std::cout << std::flush;
    tui_input_string(value, max_input);
    selected++;
}
