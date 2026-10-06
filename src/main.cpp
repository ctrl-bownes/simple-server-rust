#include <cstdio>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <ostream>
#include <termios.h>
#include <unistd.h>
#include <string>

enum class OS
{
	WINDOWS,
	LINUX,
	MAC,
	FREEBSD,
	OTHER
};

OS get_os()
{
	#ifdef _WIN32
		return OS::WINDOWS;
	#elif __APPLE__
		return OS::MAC;
	#elif __linux__
		return OS::LINUX;
	#else
		return OS::OTHER;
	#endif
}

bool extract_file(const std::string& archive, const std::string& output)
{
    std::string command = "tar -xzf \"" + archive + "\" -C \"" + output + "\"";

    if (std::system(command.c_str()) != 0)
    {
        std::cerr << "Failed to extract archive." << std::endl;
        return (false);
    }

    return (true);
}

bool download_file(const std::string& url, const std::string& output)
{
    std::string command = "curl -L \"" + url + "\" -o \"" + output + "\"";

    if (std::system(command.c_str()) != 0)
    {
        std::cerr << "Failed to download file." << std::endl;
        return (false);
    }

    return (true);
}

bool install_steamcmd()
{
    char steamcmd_url[] =
		"https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz";

    char compressed[] = "steamcmd/steamcmd_linux.tar.gz";
    try
    {
        if (std::filesystem::exists("steamcmd/steamcmd.sh"))
        {
	        std::cout << "SteamCMD is already installed." << std::endl;
        }
        else
        {
	        std::filesystem::create_directory("steamcmd");

	        if (!download_file(steamcmd_url, compressed))
		        return (false);

	        if (!extract_file(compressed, "steamcmd"))
		        return (false);
        }

        return (true);
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return (false);
    }
}

bool install_rustserver()
{
	std::string command =
        "cd steamcmd && ./steamcmd.sh "
        "+force_install_dir ../rust_server "
        "+login "
        "anonymous "
        "+app_update "
        "258550 "
        "+quit";

    if (std::system(command.c_str()) != 0)
    {
        std::cerr << "Failed to run SteamCMD." << std::endl;
        return (false);
    }
	return (true);
}

bool get_server_name(std::string &server_name)
{
    struct termios old_term;
    struct termios new_term;

    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;

    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    char c;

    while (true)
    {
        if (read(STDIN_FILENO, &c, 1) != 1)
            continue;

        // Enter
        if (c == '\n' || c == '\r')
            break;

        // Escape
        if (c == 27)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
            return false;
        }

        // Backspace
        if (c == 127 || c == 8)
        {
            if (!server_name.empty())
            {
                server_name.pop_back();

                std::cout << "\b \b" << std::flush;
            }

            continue;
        }

        // Normal character
        if (server_name.size() < 254 && c >= 32 && c <= 126)
        {
            server_name += c;
            std::cout << c << std::flush;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    return true;
}

#include "includes/tui.hpp"

// generic solution
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


#include <fstream>

bool initialize_server(std::string server_name, int world_size, int world_seed, int max_players)
{
    if (std::filesystem::exists("./rust_server/server/" + server_name +"/"))
    {
        std::cerr << "Server '" + server_name + "' already exist." << std::endl;
        return (false);
    }

    if (!std::filesystem::create_directories("./rust_server/server/" + server_name + "/cfg/"))
    {
        std::cerr << "Failed to create path './rust_server/server/" + server_name + "/cfg/" << std::endl;
    }
    std::ofstream file("./rust_server/server/" + server_name + "/cfg/server.cfg");

    if (!file)
    {
        std::cerr << "Failed to create server.cfg\n";
        return (false);
    }

    file << "server.hostname \"" << server_name << "\"\n";
    file << "server.worldsize " << world_size << "\n";
    file << "server.seed " << world_seed << "\n";
    file << "server.maxplayers " << max_players << "\n";

     std::cerr << "Successfully created " + server_name + " server.cfg.";
    return true;
// 	std::string command =
// 	    "cd rust_server && "
// 		"LD_LIBRARY_PATH=\"../steamcmd/linux64:$LD_LIBRARY_PATH\" "
//         "./RustDedicated -batchmode "
//         "+server.identity \"" + server_name + "\" "
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
	return (true);
}

int next_selection(int selected)
{
    ++selected;

    if (selected == 4)
        selected = 5;

    if (selected > 6)
        selected = 0;

    return selected;
}

int previous_selection(int selected)
{
    --selected;

    if (selected == 4)
        selected = 3;

    if (selected < 0)
        selected = 6;

    return selected;
}

bool create_server()
{
    std::string server_name = "default";
    int world_size = 1000;
    int seed = 1337;
    int max_players = 16;

    int selected = 0;

    while (true)
    {
        tui_draw_create_server(
            selected,
            server_name,
            world_size,
            seed,
            max_players
        );

        KeyEvent key = terminal_read_key();
        tui_draw_create_server(
            selected,
            server_name,
            world_size,
            seed,
            max_players
        );
        switch (key.key)
        {
            case Key::UP:
                if (selected > 0)
                {
                    if (selected == 5)
                        --selected;
                    --selected;
                }
                break;

            case Key::DOWN:
                if (selected < 6)
                {
                    ++selected;
                    if (selected == 4)
                        selected = 5;
                }
                break;

            case Key::ENTER:
            {
                if (selected == 0)
                {
                    tui_move(7, 27 + server_name.length());
                    tui_show_cursor();
                    std::cout << std::flush;

                    tui_input_string(server_name, 32);

                    if (server_name.empty())
                        server_name = "default";

                    selected = next_selection(selected);
                }
                else if (selected == 1)
                {
                    tui_move(8, 27 + numDigits(world_size));
                    tui_show_cursor();
                    std::cout << std::flush;

                    tui_input_int(world_size, 4);

                    if (world_size > 6000)
                        world_size = 6000;
                    else if (world_size < 1000)
                        world_size = 1000;

                    selected = next_selection(selected);
                }
                else if (selected == 2)
                {
                    tui_move(9, 27 + numDigits(seed));
                    tui_show_cursor();
                    std::cout << std::flush;

                    tui_input_int(seed, 10);

                    selected = next_selection(selected);
                }
                else if (selected == 3)
                {
                    tui_move(10, 27 + numDigits(max_players));
                    tui_show_cursor();
                    std::cout << std::flush;

                    tui_input_int(max_players, 4);

                    selected = next_selection(selected);
                }
                else if (selected == 5)
                {
                    // create
                }
                else if (selected == 6)
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

void server_manager()
{
	// ask user what server they want to address
	// display every server
	// 		once in a server
	//		display all information
	//		ask user what they want to do (edit config, access directory, ...) go back
}

#include "includes/tui.hpp"

void run_install()
{
    tui_clear();
    tui_show_cursor();
    terminal_restore();

    std::cout << std::flush;

    install_steamcmd();
    install_rustserver();

    std::cout << "\nPress Enter to return...";
    std::cin.get();

    terminal_raw_mode();
    tui_hide_cursor();
}

void linux_setup()
{
    terminal_raw_mode();
    tui_hide_cursor();

    int selected = 0;
    bool running = true;

    while (running)
    {
        tui_draw_main_menu(selected);

        KeyEvent key = terminal_read_key();

        switch (key.key)
        {
            case Key::UP:
                if (selected > 0)
                    --selected;
                break;

            case Key::DOWN:
                if (selected < 4)
                    ++selected;
                break;

            case Key::ENTER:
                switch (selected)
                {
                    case 0:
                        server_manager();
                        break;

                    case 1:
                        create_server();
                        break;

                    case 2:
                        run_install();
                        break;

                    case 3:
                        // open guide
                        break;

                    case 4:
                        running = false;
                        break;
                }
                break;

            case Key::CHAR:
                if (key.character == 'q' || key.character == 'Q')
                    running = false;
                break;

            case Key::CTRL_C:
                running = false;
                break;

            default:
                break;
        }
    }

    tui_show_cursor();
    terminal_restore();
    tui_clear();
}

int main(void) {
	switch (get_os())
	{
		case OS::WINDOWS:
			// std::cerr << "Error: WINDOWS OS: is not supported yet" << std::endl;
			break;

		case OS::LINUX:
			// std::cout << "OS: Linux" << std::endl;
			linux_setup();
			break;

		case OS::MAC:
			// std::cerr << "Error: MAC OS: is not supported yet" << std::endl;
			break;

		default:
			std::cerr << "Error: unrecognized OS" << std::endl;
			break;
	}
	return (0);
}
