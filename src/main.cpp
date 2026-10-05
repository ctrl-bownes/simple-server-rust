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

void linux_setup()
{
	std::cout << "OS: Linux" << std::endl;
	
	install_steamcmd();
	install_rustserver();
}


int main(void) {
	switch (get_os())
	{
		case OS::WINDOWS:
			std::cerr << "Error: WINDOWS OS: is not supported yet" << std::endl;
			break;

		case OS::LINUX:
			linux_setup();
			break;

		case OS::MAC:
			std::cerr << "Error: MAC OS: is not supported yet" << std::endl;
			break;

		default:
			std::cerr << "Error: unreconized OS" << std::endl;
			break;
	}
	return (0);
}