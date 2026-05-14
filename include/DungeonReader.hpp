#pragma once

#include <string>
#include <vector>

#include "Room.hpp"

bool read_dungeon_file(const std::string &file_name,
                       std::vector<Room> &dungeon_rooms, int &N, int &food,
                       int &target_resource, std::string &error_line);
