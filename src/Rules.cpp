#include "Rules.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

bool is_number(const std::string &s) {
  if (s.empty()) {
    return false;
  }

  for (char c : s) {
    if (!isdigit(static_cast<unsigned char>(c))) {
      return false;
    }
  }

  return true;
}

bool parse_int(const std::string &s, int &value) {
  if (!is_number(s)) {
    return false;
  }

  try {
    value = std::stoi(s);
  } catch (...) {
    return false;
  }

  return true;
}

bool parse_neighbours(const std::string &rooms, int room_number, int N,
                      std::vector<int> &neighbours) {
  neighbours.clear();

  if (rooms.empty()) {
    return false;
  }

  if (rooms.front() == ',' || rooms.back() == ',') {
    return false;
  }

  std::stringstream ss(rooms);
  std::string tmp;

  while (std::getline(ss, tmp, ',')) {
    if (tmp.empty()) {
      return false;
    }

    int x = 0;

    if (!parse_int(tmp, x)) {
      return false;
    }

    if (x < 0 || x > N) {
      return false;
    }

    if (x == room_number) {
      return false;
    }

    if (has_neighbour(neighbours, x)) {
      return false;
    }

    neighbours.push_back(x);
  }

  std::sort(neighbours.begin(), neighbours.end());

  return true;
}

bool has_neighbour(const std::vector<int> &neighbours, int x) {
  for (int y : neighbours) {
    if (y == x) {
      return true;
    }
  }

  return false;
}

bool check_graph_bidirectional(const std::vector<Room> &dungeon_rooms,
                               const std::vector<std::string> &room_lines,
                               std::string &error_line) {
  int N = static_cast<int>(dungeon_rooms.size()) - 1;

  for (int i = 0; i <= N; i++) {
    for (int to : dungeon_rooms[i].neighbours) {
      if (!has_neighbour(dungeon_rooms[to].neighbours, i)) {
        error_line = room_lines[i];
        return false;
      }
    }
  }

  return true;
}

int resource_value(int resource, int target_resource) {
  int value = 0;

  if (resource == 0) {
    value = 7;
  }

  if (resource == 1) {
    value = 11;
  }

  if (resource == 2) {
    value = 23;
  }

  if (resource == 3) {
    value = 1;
  }

  if (resource == target_resource) {
    value *= 2;
  }

  return value;
}

std::string resource_name(int resource) {
  if (resource == 0) {
    return "iron";
  }

  if (resource == 1) {
    return "gold";
  }

  if (resource == 2) {
    return "gems";
  }

  return "exp";
}

bool parse_resource_name(const std::string &s, int &resource) {
  if (s == "iron") {
    resource = 0;
    return true;
  }

  if (s == "gold") {
    resource = 1;
    return true;
  }

  if (s == "gems") {
    resource = 2;
    return true;
  }

  if (s == "exp") {
    resource = 3;
    return true;
  }

  return false;
}

int get_resource_count(const Room &r, int resource) {
  if (resource == 0) {
    return r.iron_;
  }

  if (resource == 1) {
    return r.gold_;
  }

  if (resource == 2) {
    return r.gems_;
  }

  return r.exp_;
}

int get_resource_total_value(const Room &r, int resource, int target_resource) {
  return get_resource_count(r, resource) *
         resource_value(resource, target_resource);
}

bool is_collected(const Room &r, int resource) {
  if (resource == 0) {
    return r.iron_collected;
  }

  if (resource == 1) {
    return r.gold_collected;
  }

  if (resource == 2) {
    return r.gems_collected;
  }

  return r.exp_collected;
}

void mark_collected(Room &r, int resource) {
  if (resource == 0) {
    r.iron_collected = true;
  }

  if (resource == 1) {
    r.gold_collected = true;
  }

  if (resource == 2) {
    r.gems_collected = true;
  }

  if (resource == 3) {
    r.exp_collected = true;
  }
}

bool room_has_any_collected_resource(const Room &r) {
  if (r.iron_collected) {
    return true;
  }

  if (r.gold_collected) {
    return true;
  }

  if (r.gems_collected) {
    return true;
  }

  if (r.exp_collected) {
    return true;
  }

  return false;
}
