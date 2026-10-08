#include <cstdio>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <iterator>
#include <ostream>
#include <termios.h>
#include <type_traits>
#include <unistd.h>
#include <string>

#include "includes/terminal.hpp"
#include "includes/create_server.hpp"
#include "includes/utils.hpp"
#include "includes/tui.hpp"


#include <csignal>

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
    tui_clear();
    tui_show_cursor();
    terminal_restore();

    std::cout << std::flush;

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


void server_manager()
{
	// ask user what server they want to address
	// display every server
	// 		once in a server
	//		display all information
	//		ask user what they want to do (edit config, access directory, ...) go back
}

#include "includes/tui.hpp"

void helper_terminal_restorer(bool (*function)(void))
{
    tui_clear();
    tui_show_cursor();
    terminal_restore();

    std::cout << std::flush;

    function();

    std::cout << "\nPress Enter to return...";
    std::cin.get();

    terminal_raw_mode();
    tui_hide_cursor();
}

void handle_sigin(int)
{

}

void linux_setup()
{
    int g_interrupted = 0;
    struct sigaction action = {};
    action.sa_handler = handle_sigin;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, nullptr);

    terminal_raw_mode();
    tui_hide_cursor();

    int selected = 0;

    while (!g_interrupted)
    {
        tui_draw_main_menu(selected);

        KeyEvent key = terminal_read_key();

        switch (key.key)
        {
            case Key::UP:
                if (selected > 0)
                    selected--;
                break;

            case Key::DOWN:
                if (selected < 5)
                    selected++;
                break;

            case Key::ENTER:
                switch (selected)
                {
                    case 0:
                        helper_terminal_restorer(install_steamcmd);
                        break;

                    case 1:
                        helper_terminal_restorer(install_rustserver);
                        break;

                    case 2:
                        server_manager();
                        break;

                    case 3:
                        create_server();
                        break;

                    case 4:
                        // openguide();
                        break;

                    case 5:
                        g_interrupted = true;
                        break;
                }
                break;

            case Key::ESCAPE:
                g_interrupted = true;
                break;

            case Key::CTRL_C:
                g_interrupted = true;
                break;

            default:
                break;
        }
    }

    tui_show_cursor();
    tui_clear();
    terminal_restore();
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
