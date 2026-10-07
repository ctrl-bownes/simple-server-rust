#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

const int OFFSET_Y = 5;
const int OFFSET_X = 27;

enum {
    EMPTY = 7,

    IDENTITY = 0,
    HOSTNAME = 1,
    DESCRIPTION = 2,
    WORLD_SIZE = 3,
    SEED = 4,
    MAX_PLAYERS = 5,
    PORT = 6,
    CREATE_SERVER = 8,
    CANCEL = 9
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
int next_selection(int &selected);
int previous_selection(int &selected);
void edit_value(std::string &value, int max_input, int offset_item_y, int &selected);
void edit_value(int &value, int max_input, int offset_item_y, int &selected);

#endif
