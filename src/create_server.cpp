#include "includes/create_server.hpp"
#include "includes/terminal.hpp"
#include "includes/tui.hpp"
#include "includes/utils.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>

bool create_new_server(const server_t &new_server, const std::string &server_path)
{
    std::ofstream file(server_path + "/cfg/server.cfg");
    if (!file)
    {
        std::cerr << "Failed to create server.cfg\n";
        return (false);
    }

    file << "server.identity \"" << new_server.identity << "\"\n";
    file << "server.hostname \"" << new_server.hostname << "\"\n";
    file << "server.description \"" << new_server.description << "\"\n";
    file << "server.worldsize " << new_server.world_size << "\n";
    file << "server.seed " << new_server.seed << "\n";
    file << "server.maxplayers " << new_server.max_players << "\n";
    file << "server.port " << new_server.port << "\n";

    return (true);
}

bool initialize_server(const server_t &new_server)
{
    const std::string server_path = "./rust_server/server/" + new_server.identity;
    if (std::filesystem::exists(server_path))
    {
        std::cerr << "Server '" + new_server.identity + "' already exist." << std::endl;
        return (false);
    }

    if (!std::filesystem::create_directories(server_path + "/cfg/"))
    {
        std::cerr << "Failed to create path '" + server_path + "/cfg/" << std::endl;
        return (false);
    }

    if (!create_new_server(new_server, server_path))
        return (false);

    std::cout << "Successfully created " + new_server.identity + " server.cfg.";
    return true;
}

void edit_server_value(server_t &server, int &selected)
{
    int row = server_menu_row(selected);

    if (selected == IDENTITY)
    {
        edit_value(server.identity, 32, row, selected);
        if (server.identity.empty())
            server.identity = "default";
    }
    else if (selected == HOSTNAME)
        edit_value(server.hostname, 32, row, selected);

    else if (selected == DESCRIPTION)
        edit_value(server.description, 256, row, selected);

    else if (selected == WORLD_SIZE)
    {
        edit_value(server.world_size, 4, row, selected);

        if (server.world_size > 6000)
            server.world_size = 6000;
        else if (server.world_size < 1000)
            server.world_size = 1000;
    }

    else if (selected == SEED)
        edit_value(server.seed, 10, row, selected);

    else if (selected == MAX_PLAYERS)
        edit_value(server.max_players, 4, row, selected);

    else if (selected == PORT)
        edit_value(server.port, 5, row, selected);
}

bool create_server_action(const server_t &server)
{
    terminal_restore();
    tui_show_cursor();
    tui_clear();

    bool success = initialize_server(server);

    std::cout << "\nPress Enter to return...";
    std::cin.get();

    terminal_raw_mode();
    tui_hide_cursor();

    return success;
}

bool create_server()
{
    server_t new_server;
    int selected = 0;

    while (true)
    {
        tui_draw_create_server(selected, new_server);

        KeyEvent key = terminal_read_key();
        switch (key.key)
        {
            case Key::UP:
                if (selected > 0)
                    selected--;
                break;

            case Key::DOWN:
                if (selected < 8)
                    selected++;
                break;

            case Key::ENTER:
            {
                if (selected == CREATE_SERVER)
                {
                    if (create_server_action(new_server))
                        return true;
                }
                else if (selected == CANCEL)
                    return false;
                else
                    edit_server_value(new_server, selected);
                break;
            }

            case Key::ESCAPE:
                return false;

            case Key::CTRL_C:
                return false;

            default:
                break;
        }
    }
}

// 	std::string command =
// 	    "cd rust_server && "
// 		"LD_LIBRARY_PATH=\"../steamcmd/linux64:$LD_LIBRARY_PATH\" "
//         "./RustDedicated -batchmode "
//         "+server.identity \"" + identity + "\" "
//         "+server.worldsize " + std::to_string(world_size) + " "
//         "+server.seed " + std::to_string(world_seed) + " "
//         "+server.maxplayers " + std::to_string(max_player) + " "
//         "+quit";
//
//     if (std::system(command.c_str()) != 0)
//     {
//         std::cerr << "Failed to run SteamCMD." << std::endl;
//         return (false);
//     }
// return (true);
