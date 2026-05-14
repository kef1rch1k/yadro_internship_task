#include "AliceBot.hpp"
#include "Rules.hpp"

#include <algorithm>
#include <queue>

AliceBot::AliceBot(std::vector<Room> &dungeon_rooms, int N, int food,
                   int target_resource, std::ostream &result)
    : Bot(dungeon_rooms, N, food, target_resource, result) {}

void AliceBot::run() {
  int start_food = food_;
  int exploration_food = start_food / 2;
  int spent_exploration_food = 0;

  int current_room = 0;

  std::vector<bool> visited(N_ + 1, false);
  visited[current_room] = true;

  while (spent_exploration_food < exploration_food) {
    int next_room = find_next_room_for_exploration(visited, current_room);

    if (next_room == -1) {
      break;
    }

    food_--;
    spent_exploration_food++;

    current_room = next_room;
    visited[current_room] = true;

    write_go(current_room);
    write_state(dungeon_rooms_[current_room]);

    int best_resource = choose_best_resource(dungeon_rooms_[current_room]);

    if (best_resource != -1) {
      int cost = 0;

      if (room_has_any_collected_resource(dungeon_rooms_[current_room])) {
        cost = 1;
      }

      if (spent_exploration_food + cost <= exploration_food) {
        if (food_ >= cost) {
          food_ -= cost;
          spent_exploration_food += cost;

          collect_resource_with_output(current_room, best_resource);
        }
      }
    }
  }

  while (current_room != 0) {
    collect_extra_on_return(visited, current_room);

    std::vector<int> path = find_path_to_start(visited, current_room);

    if (path.size() < 2) {
      break;
    }

    int next_room = path[1];

    food_--;
    current_room = next_room;

    write_go(current_room);

    if (current_room != 0) {
      write_state(dungeon_rooms_[current_room]);
    }
  }

  write_result();
}

int AliceBot::choose_best_resource(const Room &room) const {
  int best_resource = -1;
  int best_value = -1;

  for (int resource = 0; resource < 4; resource++) {
    if (get_resource_count(room, resource) == 0) {
      continue;
    }

    if (is_collected(room, resource)) {
      continue;
    }

    int value = get_resource_count(room, resource) *
                resource_value(resource, target_resource_);

    if (value > best_value) {
      best_value = value;
      best_resource = resource;
    }
  }

  return best_resource;
}

int AliceBot::find_next_room_for_exploration(const std::vector<bool> &visited,
                                             int current_room) const {
  int next_room = -1;

  for (int room : dungeon_rooms_[current_room].neighbours) {
    if (!visited[room]) {
      if (next_room == -1 || room < next_room) {
        next_room = room;
      }
    }
  }

  if (next_room != -1) {
    return next_room;
  }

  std::vector<int> dist(N_ + 1, -1);
  std::vector<int> parent(N_ + 1, -1);

  std::queue<int> q;

  q.push(current_room);
  dist[current_room] = 0;

  while (!q.empty()) {
    int v = q.front();
    q.pop();

    for (int to : dungeon_rooms_[v].neighbours) {
      if (dist[to] != -1) {
        continue;
      }

      dist[to] = dist[v] + 1;
      parent[to] = v;
      q.push(to);
    }
  }

  int target = -1;

  for (int i = 0; i <= N_; i++) {
    if (visited[i]) {
      continue;
    }

    if (dist[i] == -1) {
      continue;
    }

    if (target == -1) {
      target = i;
    } else {
      if (dist[i] < dist[target]) {
        target = i;
      } else if (dist[i] == dist[target] && i < target) {
        target = i;
      }
    }
  }

  if (target == -1) {
    return -1;
  }

  std::vector<int> path;
  int cur = target;

  while (cur != -1) {
    path.push_back(cur);
    cur = parent[cur];
  }

  std::reverse(path.begin(), path.end());

  if (path.size() < 2) {
    return -1;
  }

  return path[1];
}

std::vector<int> AliceBot::find_path_to_start(const std::vector<bool> &visited,
                                              int current_room) const {
  std::vector<int> dist(N_ + 1, -1);

  std::queue<int> q;

  q.push(0);
  dist[0] = 0;

  while (!q.empty()) {
    int v = q.front();
    q.pop();

    for (int to : dungeon_rooms_[v].neighbours) {
      if (!visited[to]) {
        continue;
      }

      if (dist[to] != -1) {
        continue;
      }

      dist[to] = dist[v] + 1;
      q.push(to);
    }
  }

  std::vector<int> path;

  if (dist[current_room] == -1) {
    return path;
  }

  int cur = current_room;
  path.push_back(cur);

  while (cur != 0) {
    int next_room = -1;

    for (int to : dungeon_rooms_[cur].neighbours) {
      if (!visited[to]) {
        continue;
      }

      if (dist[to] != dist[cur] - 1) {
        continue;
      }

      if (next_room == -1 || to < next_room) {
        next_room = to;
      }
    }

    if (next_room == -1) {
      path.clear();
      return path;
    }

    cur = next_room;
    path.push_back(cur);
  }

  return path;
}

void AliceBot::collect_resource_with_output(int room_number, int resource) {
  collect_resource(dungeon_rooms_[room_number], resource);

  write_collect(resource);
  write_state(dungeon_rooms_[room_number]);
}

void AliceBot::collect_extra_on_return(const std::vector<bool> &visited,
                                       int current_room) {
  while (true) {
    std::vector<int> path = find_path_to_start(visited, current_room);

    if (path.empty()) {
      return;
    }

    int need_food_to_start = static_cast<int>(path.size()) - 1;

    if (food_ <= need_food_to_start) {
      return;
    }

    int best_resource = choose_best_resource(dungeon_rooms_[current_room]);

    if (best_resource == -1) {
      return;
    }

    int cost = 0;

    if (room_has_any_collected_resource(dungeon_rooms_[current_room])) {
      cost = 1;
    }

    if (food_ - cost < need_food_to_start) {
      return;
    }

    food_ -= cost;

    collect_resource_with_output(current_room, best_resource);
  }
}
