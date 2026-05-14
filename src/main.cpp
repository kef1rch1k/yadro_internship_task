#include <fstream>
#include <string>
#include <vector>

#include "AliceBot.hpp"
#include "DungeonReader.hpp"
#include "Room.hpp"

int main(int argc, char *argv[]) {
  std::ofstream result("result.txt");

  if (!result.is_open()) {
    return 1;
  }

  if (argc != 2) {
    return 0;
  }

  std::vector<Room> dungeon_rooms;

  int N = 0;
  int food = 0;
  int target_resource = 0;

  std::string error_line;

  bool ok = read_dungeon_file(argv[1], dungeon_rooms, N, food, target_resource,
                              error_line);

  if (!ok) {
    if (!error_line.empty()) {
      result << error_line << "\n";
    }

    return 0;
  }

  AliceBot bot(dungeon_rooms, N, food, target_resource, result);
  bot.run();

  return 0;
}
