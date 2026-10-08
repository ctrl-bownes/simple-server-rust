#include "includes/tui.hpp"
#include "includes/utils.hpp"
#include "includes/terminal.hpp"

#include <cstddef>
#include <cstring>
#include <iostream>
#include <climits>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <unistd.h>

void tui_clear()
{
    std::cout << "\033[2J\033[H";
}

void tui_move(int row, int column)
{
    std::cout << "\033[" << row << ";" << column << "H";
}

int tui_display_width(const std::string &str)
{
    int width = 0;

    for (std::size_t i = 0; i < str.length(); ++i)
    {
        if ((str[i] & 0xC0) != 0x80)
            ++width;
    }

    return width;
}

void tui_move_center_width(const std::string &str, int row)
{
    int width = tui_display_width(str);
    int column = (WIDTH - width) / 2 + 1;

    tui_move(row, column);
}

void tui_hide_cursor()
{
    std::cout << "\033[?25l";
}

void tui_show_cursor()
{
    std::cout << "\033[?25h";
}

void tui_draw_box(
    int top,
    int left,
    int width,
    int height
)
{
    tui_move(top, left);
    std::cout << "╭";

    for (int i = 0; i < width - 2; ++i)
        std::cout << "─";

    std::cout << "╮";

    for (int row = 1; row < height - 1; ++row)
    {
        tui_move(top + row, left);
        std::cout << "│";

        tui_move(top + row, left + width - 1);
        std::cout << "│";
    }

    tui_move(top + height - 1, left);
    std::cout << "╰";

    for (int i = 0; i < width - 2; ++i)
        std::cout << "─";

    std::cout << "╯";

    std::string keys = "'↑' '↓'  Navigate    'Enter'  Edit    'Esc'  Back";
    tui_move_center_width(keys, HEIGHT - 2);

    std::cout << "\033[2m" << keys << "\033[0m";
}

#include <vector>

void tui_draw_main_menu(int selected)
{
    std::vector<int> offset;
    tui_clear();
    tui_draw_box(1, 1, WIDTH, HEIGHT);

    std::string str = "SIMPLE RUST SERVER MANAGER";
    tui_move_center_width(str, 3);
    std::cout << "\033[4m" << str << "\033[0m";

    const char *items[] =
    {
        "Install steamCMD",
        "Install/Update rust_server_files",
        "Manage Servers",
        "Create Server",
        "Open Guide",
        "Exit"
    };

    offset = {5, 8};
    int n_space = 0;
    for (int i = 0; i < static_cast<int>(sizeof(items)/sizeof(items[0])); ++i)
    {
        int row = i + n_space;
//
        if (i == 2 || i == 5)
        {
            row++;
            n_space++;
        }

        tui_move(row + offset[0], offset[1]);

        if (i == selected)
            std::cout << "> ";
        else
            std::cout << "  ";

        std::cout << items[i];
    }
    std::cout << std::flush;

}

void tui_draw_create_server(int selected, const server_t &new_server)
{
    tui_clear();
    tui_draw_box(1, 1, WIDTH, HEIGHT);
    tui_move(3, 21);
    std::string title = "CREATE SERVER";
    tui_move_center_width(title, 3);

    std::cout << "\033[4m" << title << "\033[0m";

    const char *items[] =
    {
        "Identity",
        "Hostname",
        "Description",
        "World size",
        "Seed",
        "Max players",
        "port",
        "Create Server",
        "Cancel"
    };

    int n_space = 0;
    for (int i = 0; i < static_cast<int>(sizeof(items)/sizeof(items[0])); ++i)
    {
        int row = i + n_space;

        if (i == 3 || i == 7)
        {
            row++;
            n_space++;
        }
        tui_move(5 + row, 7);

        if (i == selected)
            std::cout << "> ";
        else
            std::cout << "  ";

        std::cout << items[i];

        tui_move(5 + row, 27);

        switch (i)
        {
            case IDENTITY:
                std::cout << new_server.identity;
                break;

            case HOSTNAME:
                std::cout << new_server.hostname;
                break;

            case DESCRIPTION:
                std::cout << new_server.description;
                break;

            case WORLD_SIZE:
                std::cout << new_server.world_size;
                break;

            case SEED:
                std::cout << new_server.seed;
                break;

            case MAX_PLAYERS:
                std::cout << new_server.max_players;
                break;

            case PORT:
                std::cout << new_server.port;
                break;
        }
    }

    std::cout << std::flush;
}

bool tui_input_string(std::string &value, std::size_t max_length)
{
    tui_show_cursor();

    while (true)
    {
        KeyEvent key = terminal_read_key();

        if (key.key == Key::ENTER)
        {
            tui_hide_cursor();
            return true;
        }

        if (key.key == Key::ESCAPE)
        {
            tui_hide_cursor();
            return false;
        }

        if (key.key == Key::BACKSPACE)
        {
            if (!value.empty())
            {
                value.pop_back();

                std::cout << "\b \b" << std::flush;
            }

            continue;
        }

        if (key.key == Key::CHAR)
        {
            if (value.size() < max_length
                && key.character >= 32
                && key.character <= 126)
            {
                value += key.character;
                std::cout << key.character << std::flush;
            }
        }
    }
}

bool tui_input_int(int &value, std::size_t max_length)
{
    std::string input = std::to_string(value);

    tui_show_cursor();

    while (true)
    {
        KeyEvent key = terminal_read_key();

        if (key.key == Key::ENTER)
        {
            long temp = std::strtol(input.c_str(), NULL, 10);

            if (temp > INT_MAX)
                value = INT_MAX;
            else
                value = static_cast<int>(temp);

            tui_hide_cursor();
            return true;
        }

        if (key.key == Key::ESCAPE)
        {
            tui_hide_cursor();
            return false;
        }

        if (key.key == Key::BACKSPACE)
        {
            if (!input.empty())
            {
                input.pop_back();
                std::cout << "\b \b" << std::flush;
            }

            continue;
        }

        if (key.key == Key::CHAR)
        {
            if (input.size() < max_length
                && key.character >= '0'
                && key.character <= '9')
            {
                input += key.character;
                std::cout << key.character << std::flush;
            }
        }
    }
}
