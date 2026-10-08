#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

const int OFFSET_Y = 5;
const int OFFSET_X = 27;

enum {
    IDENTITY,
    HOSTNAME,
    DESCRIPTION,
    WORLD_SIZE,
    SEED,
    MAX_PLAYERS,
    PORT,
    CREATE_SERVER,
    CANCEL,
};

typedef struct server_s
{
    std::string identity = "default";
    std::string hostname = "My Rust Server";
    std::string description = "My Description";
    int world_size = 1000;
    int seed = 1337;
    int max_players = 16;
    int port = 28015;
}   server_t;

template <class T> int numDigits(T number);
int server_menu_row(int &selected);
void edit_value(std::string &value, int max_input, int offset_item_y, int &selected);
void edit_value(int &value, int max_input, int offset_item_y, int &selected);

#endif
