#include "Bot.hpp"
#include "Rules.hpp"

#include <ostream>

Bot::Bot(std::vector<Room> &dungeon_rooms, int N, int food, int target_resource,
         std::ostream &result)
    : dungeon_rooms_(dungeon_rooms), N_(N), food_(food),
      target_resource_(target_resource), result_(result), collected_(4, 0) {}

void Bot::write_go(int room_number) { result_ << "go " << room_number << "\n"; }

void Bot::write_collect(int resource) {
  result_ << "collect " << resource_name(resource) << "\n";
}

void Bot::write_state(const Room &room) {
  result_ << "state " << room.room_number << " ";

  if (room.iron_collected) {
    result_ << "_";
  } else {
    result_ << room.iron_;
  }

  result_ << " ";

  if (room.gold_collected) {
    result_ << "_";
  } else {
    result_ << room.gold_;
  }

  result_ << " ";

  if (room.gems_collected) {
    result_ << "_";
  } else {
    result_ << room.gems_;
  }

  result_ << " ";

  if (room.exp_collected) {
    result_ << "_";
  } else {
    result_ << room.exp_;
  }

  result_ << "\n";
}

void Bot::write_result() {
  int treasure = 0;

  for (int resource = 0; resource < 4; resource++) {
    treasure +=
        collected_[resource] * resource_value(resource, target_resource_);
  }

  result_ << "result " << collected_[0] << " " << collected_[1] << " "
          << collected_[2] << " " << collected_[3] << " " << treasure << "\n";
}

void Bot::collect_resource(Room &room, int resource) {
  int count = get_resource_count(room, resource);

  collected_[resource] += count;
  mark_collected(room, resource);
}
