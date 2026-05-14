#include "DungeonReader.hpp"
#include "Rules.hpp"

#include <fstream>
#include <sstream>

bool read_dungeon_file(const std::string &file_name,
                       std::vector<Room> &dungeon_rooms, int &N, int &food,
                       int &target_resource, std::string &error_line) {
  std::ifstream input(file_name);

  if (!input.is_open()) {
    error_line = file_name;
    return false;
  }

  std::string line;

  if (!std::getline(input, line)) {
    return false;
  }

  if (!parse_int(line, N)) {
    error_line = line;
    return false;
  }

  if (N < 1 || N > 255) {
    error_line = line;
    return false;
  }

  dungeon_rooms.clear();
  dungeon_rooms.resize(N + 1);

  std::vector<bool> used_room(N + 1, false);
  std::vector<std::string> room_lines(N + 1);

  for (int i = 0; i < N + 1; i++) {
    if (!std::getline(input, line)) {
      return false;
    }

    std::stringstream ss(line);

    std::vector<std::string> parts;
    std::string part;

    while (ss >> part) {
      parts.push_back(part);
    }

    if (parts.size() != 2 && parts.size() != 6) {
      error_line = line;
      return false;
    }

    std::string room_number_string = parts[0];
    std::string rooms = parts[1];

    std::string iron_string = "0";
    std::string gold_string = "0";
    std::string gems_string = "0";
    std::string exp_string = "0";

    if (parts.size() == 6) {
      iron_string = parts[2];
      gold_string = parts[3];
      gems_string = parts[4];
      exp_string = parts[5];
    }

    Room r;

    int room_number = 0;
    int iron = 0;
    int gold = 0;
    int gems = 0;
    int exp = 0;

    if (!parse_int(room_number_string, room_number)) {
      error_line = line;
      return false;
    }

    if (room_number < 0 || room_number > N) {
      error_line = line;
      return false;
    }

    if (room_number != 0 && parts.size() == 2) {
      error_line = line;
      return false;
    }

    if (used_room[room_number]) {
      error_line = line;
      return false;
    }

    if (!parse_neighbours(rooms, room_number, N, r.neighbours)) {
      error_line = line;
      return false;
    }

    if (!parse_int(iron_string, iron)) {
      error_line = line;
      return false;
    }

    if (!parse_int(gold_string, gold)) {
      error_line = line;
      return false;
    }

    if (!parse_int(gems_string, gems)) {
      error_line = line;
      return false;
    }

    if (!parse_int(exp_string, exp)) {
      error_line = line;
      return false;
    }

    if (iron < 0 || iron > 255) {
      error_line = line;
      return false;
    }

    if (gold < 0 || gold > 255) {
      error_line = line;
      return false;
    }

    if (gems < 0 || gems > 255) {
      error_line = line;
      return false;
    }

    if (exp < 0 || exp > 255) {
      error_line = line;
      return false;
    }

    if (room_number == 0) {
      if (iron != 0 || gold != 0 || gems != 0 || exp != 0) {
        error_line = line;
        return false;
      }
    }

    r.room_number = room_number;
    r.iron_ = iron;
    r.gold_ = gold;
    r.gems_ = gems;
    r.exp_ = exp;

    r.iron_collected = false;
    r.gold_collected = false;
    r.gems_collected = false;
    r.exp_collected = false;

    dungeon_rooms[room_number] = r;
    used_room[room_number] = true;
    room_lines[room_number] = line;
  }

  for (int i = 0; i <= N; i++) {
    if (!used_room[i]) {
      return false;
    }
  }

  if (!check_graph_bidirectional(dungeon_rooms, room_lines, error_line)) {
    return false;
  }

  if (!std::getline(input, line)) {
    return false;
  }

  std::stringstream last_line(line);

  std::string food_string;
  std::string target_string;
  std::string extra;

  last_line >> food_string >> target_string;

  if (last_line >> extra) {
    error_line = line;
    return false;
  }

  if (food_string.empty() || target_string.empty()) {
    error_line = line;
    return false;
  }

  if (!parse_int(food_string, food)) {
    error_line = line;
    return false;
  }

  if (food < 2 || food > 255) {
    error_line = line;
    return false;
  }

  if (!parse_resource_name(target_string, target_resource)) {
    error_line = line;
    return false;
  }

  std::string extra_line;

  if (std::getline(input, extra_line)) {
    error_line = extra_line;
    return false;
  }

  return true;
}
