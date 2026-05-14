#pragma once

#include <vector>

struct Room {
  int room_number;
  std::vector<int> neighbours;

  int iron_;
  int gold_;
  int gems_;
  int exp_;

  bool iron_collected;
  bool gold_collected;
  bool gems_collected;
  bool exp_collected;
};
