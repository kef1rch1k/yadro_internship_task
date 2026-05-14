#pragma once

#include <string>
#include <vector>

#include "Room.hpp"

bool is_number(const std::string &s);

bool parse_int(const std::string &s, int &value);
bool parse_neighbours(const std::string &rooms, int room_number, int N,
                      std::vector<int> &neighbours);

bool has_neighbour(const std::vector<int> &neighbours, int x);

bool check_graph_bidirectional(const std::vector<Room> &dungeon_rooms,
                               const std::vector<std::string> &room_lines,
                               std::string &error_line);

int resource_value(int resource, int target_resource);
std::string resource_name(int resource);

bool parse_resource_name(const std::string &s, int &resource);

int get_resource_count(const Room &r, int resource);
int get_resource_total_value(const Room &r, int resource, int target_resource);

bool is_collected(const Room &r, int resource);
void mark_collected(Room &r, int resource);
bool room_has_any_collected_resource(const Room &r);
