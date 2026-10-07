#include "includes/create_server.hpp"

bool initialize_server(const server_t &server_cfg)
{
    const std::string server_path = "./rust_server/server/" + server_cfg.identity;
    if (std::filesystem::exists(server_path))
    {
        std::cerr << "Server '" + server_cfg.identity + "' already exist." << std::endl;
        return (false);
    }

    if (!std::filesystem::create_directories(server_path + "/cfg/"))
    {
        std::cerr << "Failed to create path '" + server_path + "/cfg/" << std::endl;
    }

    std::ofstream file(server_path + "/cfg/server.cfg");
    if (!file)
    {
        std::cerr << "Failed to create server.cfg\n";
        return (false);
    }

    file << "server.identity \"" << server_cfg.identity << "\"\n";
    file << "server.hostname \"" << server_cfg.hostname << "\"\n";
    file << "server.description \"" << server_cfg.description << "\"\n";
    file << "server.worldsize " << server_cfg.world_size << "\n";
    file << "server.seed " << server_cfg.seed << "\n";
    file << "server.maxplayers " << server_cfg.max_players << "\n";
    file << "server.port " << server_cfg.port << "\n";

    std::cout << "Successfully created " + server_cfg.identity + " server.cfg.";
    return true;
}

bool create_server()
{
    server_t new_server;
    int selected = 0;

    while (true)
    {
        tui_draw_create_server(
            selected,
            new_server.identity,
            new_server.hostname,
            new_server.description,
            new_server.world_size,
            new_server.seed,
            new_server.max_players,
            new_server.port
        );

        KeyEvent key = terminal_read_key();
        switch (key.key)
        {
            case Key::UP:
                if (selected > 0)
                    selected = previous_selection(selected);
                break;

            case Key::DOWN:
                if (selected < 9)
                    selected = next_selection(selected);
                break;

            case Key::ENTER:
            {
                if (selected == IDENTITY)
                {
                    edit_value(new_server.identity, 32, IDENTITY, selected);
                    if (new_server.identity.empty())
                        new_server.identity = "default";
                }

                else if (selected == HOSTNAME)
                    edit_value(new_server.hostname, 32, HOSTNAME, selected);

                else if (selected == DESCRIPTION)
                    edit_value(new_server.description, 256, DESCRIPTION, selected);

                else if (selected == WORLD_SIZE)
                {
                    edit_value(new_server.world_size, 4, WORLD_SIZE, selected);
                    if (new_server.world_size > 6000)
                        new_server.world_size = 6000;
                    else if (new_server.world_size < 1000)
                        new_server.world_size = 1000;
                }

                else if (selected == SEED)
                    edit_value(new_server.seed, 10, SEED, selected);

                else if (selected == MAX_PLAYERS)
                    edit_value(new_server.max_players, 4, MAX_PLAYERS, selected);

                else if (selected == PORT)
                    edit_value(new_server.port, 5, PORT, selected);

                else if (selected == CREATE_SERVER)
                {
                    terminal_restore();
                    tui_show_cursor();
                    tui_clear();

                    bool success = initialize_server(new_server);
                    std::cout << "\nPress Enter to return...";
                    std::cin.get();
                    terminal_raw_mode();
                    tui_hide_cursor();
                    if (!success)
                        break;
                    return true;
                }
                else if (selected == CANCEL)
                {
                    return false;
                }

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
