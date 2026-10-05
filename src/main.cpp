#include <iostream>
#include <filesystem>
#include <cstdlib>

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
	#elif __FreeBSD__
		return OS::FREEBSD;
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
    try
    {
        if (std::filesystem::exists("steamcmd/steamcmd.sh"))
        {
            std::cout << "SteamCMD is already installed." << std::endl;
        }
		else
		{
			std::filesystem::create_directory("steamcmd");

			if (!download_file("https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz", "steamcmd/steamcmd_linux.tar.gz"))
				return (false);

			if (!extract_file("steamcmd/steamcmd_linux.tar.gz", "steamcmd"))
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
	std::string command = "cd steamcmd && ./steamcmd.sh +force_install_dir ../rust_server +login anonymous +app_update 258550 +quit";

    if (std::system(command.c_str()) != 0)
    {
        std::cerr << "Failed to run SteamCMD." << std::endl;
        return (false);
    }
	return (true);
}

enum {
	INSTALL,
	CREATE,
	MANAGE,
};

bool launch_rustserver()
{
	// ask user for name
	// seed, size etc
	// tell them that they will still be ablt to change that later
	// check that server with seame identity exist

    std::string command =
        "cd rust_server && "
        "LD_LIBRARY_PATH=\"../steamcmd/linux64:$LD_LIBRARY_PATH\" "
        "./RustDedicated "
        "+server.identity \"my_server\" "
        "+server.hostname \"My Test Server\" "
        "+server.port 28015 "
        "+server.level \"Procedural Map\" "
        "+server.worldsize 1000 "
        "+server.seed 1337 "
        "+server.maxplayers 10";

    if (std::system(command.c_str()) != 0)
    {
        std::cerr << "Failed to launch Rust server." << std::endl;
        return false;
    }

    return true;
}

void server_manager()
{
	// ask user what server they want to address
	// display every server
	// 		once in a server
	//		display all information 
	//		ask user what they want to do (edit config, access directory, ...) go back
}

void linux_setup()
{
	int choice;
	char *end;
	long value;

	std::cout 
		<< "Setup"
		<< "\n"
		<< "-----"
		<< "\n"
		<< "\n" << "0 - Install steamcmd"
		<< "\n" << "1 - Create a server"
		<< "\n" << "2 - Manage Servers" 
		<< "\n" << "3 - Exit" 
		<< "\n"
		<< "\n" << "choice: ";

	
	std::string input;
	std::getline(std::cin, input);
	
	if (input.empty())
		return;

	value = std::strtol(input.c_str(), &end, 10);

	choice = -1;
	if (*end == '\0')
		choice = static_cast<int>(value);

	switch (choice)
	{
	case INSTALL:
		install_steamcmd();
		break;

	case CREATE:
		launch_rustserver();
		break;
	
	case MANAGE:
		server_manager();
		break;
	
	default:
		break;
	}
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
			std::cerr << "Error: unreconized OS" << std::endl;
			break;
	}
	return (0);
}