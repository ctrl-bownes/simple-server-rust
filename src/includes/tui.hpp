#ifndef TUI_HPP
#define TUI_HPP

#include <iterator>
#include <string>

#include "terminal.hpp"

void tui_clear();
void tui_move(int row, int column);
void tui_hide_cursor();
void tui_show_cursor();

void tui_draw_box(
    int top,
    int left,
    int width,
    int height
);

void tui_draw_main_menu(int selected);

void tui_draw_create_server(
    int selected,
    const std::string &server_name,
    int world_size,
    int seed,
    int max_players
);

bool tui_input_string(std::string &value, std::size_t max_length);
bool tui_input_int(int &value, std::size_t max_length);

#endif
