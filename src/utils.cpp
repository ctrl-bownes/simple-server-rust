#include "includes/utils.hpp"

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

int next_selection(int selected)
{
    ++selected;
    if (selected == EMPTY)
        ++selected;
    return selected;
}

int previous_selection(int selected)
{
    --selected;
    if (selected == EMPTY)
        --selected;
    return selected;
}

void edit_value(int &value, int max_input, int offset_item_y, int &selected)
{
    tui_move(OFFSET_Y + offset_item_y, OFFSET_X + numDigits(value));
    tui_show_cursor();
    std::cout << std::flush;
    tui_input_int(value, max_input);
    selected = next_selection(selected);
}

void edit_value(std::string &value, int max_input, int offset_item_y, int &selected)
{
    tui_move(OFFSET_Y + offset_item_y, OFFSET_X + value.length());
    tui_show_cursor();
    std::cout << std::flush;
    tui_input_string(value, max_input);
    selected = next_selection(selected);
}
