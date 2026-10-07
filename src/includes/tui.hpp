#ifndef TUI_HPP
#define TUI_HPP

#include <iterator>
#include <string>

#include "terminal.hpp"

void tui_clear();
void tui_move(int row, int column);
void tui_hide_cursor();
void tui_show_cursor();

enum {
    EMPTY = 7,
    IDENTITY = 0,
    HOSTNAME = 1,
    DESCRIPTION = 2,
    WORLD_SIZE = 3,
    SEED = 4,
    MAX_PLAYERS = 5,
    PORT = 6,
    CREATE_SERVER = 8,
    CANCEL = 9
};

void tui_draw_box(
    int top,
    int left,
    int width,
    int height
);

void tui_draw_main_menu(int selected);

void tui_draw_create_server(
    int selected,
    const std::string &identity,
    const std::string &hostname,
    const std::string &description,
    int world_size,
    int seed,
    int max_players,
    int port
);

bool tui_input_string(std::string &value, std::size_t max_length);
bool tui_input_int(int &value, std::size_t max_length);

#endif
