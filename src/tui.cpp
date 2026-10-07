#include "includes/tui.hpp"
#include "includes/utils.hpp"
#include "includes/terminal.hpp"

#include <iostream>
#include <climits>
#include <cstdlib>
#include <unistd.h>

void tui_clear()
{
    std::cout << "\033[2J\033[H";
}

void tui_move(int row, int column)
{
    std::cout << "\033[" << row << ";" << column << "H";
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
}

void tui_draw_main_menu(int selected)
{
    tui_clear();

    tui_draw_box(1, 1, 62, 19);

    tui_move(3, 17);
    std::cout << "SIMPLE RUST SERVER MANAGER";

    const char *items[] =
    {
        "Manage Servers",
        "Create Server",
        "Install / Update Rust",
        "Open Guide",
        "Exit"
    };

    for (int i = 0; i < 5; ++i)
    {
        tui_move(7 + i, 8);

        if (i == selected)
            std::cout << "> ";
        else
            std::cout << "  ";

        std::cout << items[i];
    }

    tui_move(16, 8);
    std::cout << "'↑' '↓'  Navigate";

    tui_move(17, 8);
    std::cout << "'Enter'  Select     'Q'  Quit";

    std::cout << std::flush;
}

void tui_draw_create_server(int selected, const server_t &new_server)
{
    tui_clear();
    tui_draw_box(1, 1, 62, 19);
    tui_move(3, 21);
    std::cout << "CREATE SERVER";

    const char *items[] =
    {
        "Identity",
        "Hostname",
        "Description",
        "World size",
        "Seed",
        "Max players",
        "port",
        "",
        "Create Server",
        "Cancel"
    };

    for (int i = 0; i < 10; ++i)
    {
        tui_move(5 + i, 7);

        if (i == selected)
            std::cout << "> ";
        else
            std::cout << "  ";

        std::cout << items[i];

        tui_move(5 + i, 27);

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

    tui_move(16, 7);
    std::cout << "'↑' '↓'  Navigate    'Enter'  Edit    'Esc'  Back";

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
