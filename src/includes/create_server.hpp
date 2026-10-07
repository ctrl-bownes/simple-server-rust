#ifndef CREATE_SERVER_HPP
#define CREATE_SERVER_HPP

#include "utils.hpp"

bool create_server_cfg(const server_t &server_cfg, const std::string &server_path);
bool initialize_server(const server_t &server_cfg);
bool create_server();

#endif
