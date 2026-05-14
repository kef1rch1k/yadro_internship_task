#pragma once

#include "Bot.hpp"

#include <vector>

class AliceBot : public Bot {
public:
  AliceBot(std::vector<Room> &dungeon_rooms, int N, int food,
           int target_resource, std::ostream &result);

  void run() override;

private:
  int choose_best_resource(const Room &room) const;

  int find_next_room_for_exploration(const std::vector<bool> &visited,
                                     int current_room) const;
  std::vector<int> find_path_to_start(const std::vector<bool> &visited,
                                      int current_room) const;

  void collect_resource_with_output(int room_number, int resource);
  void collect_extra_on_return(const std::vector<bool> &visited,
                               int current_room);
};
