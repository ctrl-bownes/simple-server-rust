#include <iostream>

std::string get_os()
{
	#ifdef _WIN32
		return "windows";
	#elif _WIN64
		return "windows";
	#elif __APPLE__ || __MACH__
		return "mac";
	#elif __linux__
		return "linux";
	#elif __FreeBSD__
		return "freebsd";
	#elif __unix || __unix__
		return "unix";
	#else
		return "other";
	#endif
}


int main(void) {
	std::cout << get_os() << std::endl;
	return (0);
}