#pragma once

#include <iosfwd>
#include <vector>

#include "Room.hpp"

class Bot {
public:
  Bot(std::vector<Room> &dungeon_rooms, int N, int food, int target_resource,
      std::ostream &result);
  virtual ~Bot() = default;

  virtual void run() = 0;

protected:
  std::vector<Room> &dungeon_rooms_;
  int N_;
  int food_;
  int target_resource_;
  std::ostream &result_;

  std::vector<int> collected_;

  void write_go(int room_number);
  void write_collect(int resource);
  void write_state(const Room &room);
  void write_result();

  void collect_resource(Room &room, int resource);
};
