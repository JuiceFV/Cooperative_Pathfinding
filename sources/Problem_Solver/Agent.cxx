#include "Agent.hpp"

char Agent::get_name() const { return (name); }

void Agent::set_start(const Node& s) {
  start = s;
  path.push_back(s);
  current_node = s;
}

Node Agent::get_start() const { return (start); }

void Agent::set_goal(const Node& g) { goal = g; }

Node Agent::get_goal() const { return (goal); }

void Agent::set_whole_path() {
  Node current = start;
  path = {current};
  auto search = came_from.find(current);
  auto finish = came_from.end();
  while (search != finish) {
    current = came_from[current];
    path.push_back(current);
    if (current == goal) break;
  }
}

void Agent::get_path(std::list<Node>& p) {
  p.clear();
  for (auto& node : path) p.push_back(node);
}

void Agent::set_portion_path(
    const std::vector<std::unordered_map<Node, Agent*>>& space_map) {
  portion_path.clear();

  uint steps_left = WINDOW_SIZE;
  Node current = current_node;
  Node prev = current_node;
  Node next_best = current;

  if (current == goal) {
    for (uint i = 0; i < WINDOW_SIZE; ++i) portion_path.push_back(goal);
    return;
  }
  for (auto i = 0; i < WINDOW_SIZE; ++i) {
    if (current == goal) break;
    if (came_from.find(current) == came_from.end()) break;

    next_best = came_from[current];
    // First case:  next best node is not occupied, but is it safe for me to go
    // there?
    if (space_map[i].find(next_best) != space_map[i + 1].end()) {
      auto search1 = space_map[i].find(current);
      if (search1 != space_map[i].end()) {
        Agent* another_agent = search1->second;

        auto search2 = space_map[i - 1].find(next_best);
        if (search1->second == search2->second) {
          uint x = current.x;
          uint y = current.y;

          vector<Node> neighbors = {
              {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
          uint current_best_g_score = g_score[current];
          uint next_best_g_score = numeric_limits<uint>::max();

          for (auto& neighbor : neighbors) {
            if (neighbor == next_best)
              continue;  // continue becasue, next_best_node is to be avoided..
            if (neighbor == prev)
              continue;  //  we dont want agent go back. sometimes going back is
                         //  the only option.

            if (g_score[neighbor] < next_best_g_score &&
                space_map[i].find(neighbor) ==
                    space_map[i]
                        .end())  // find best g_score and it is not in space_map
            {
              next_best = neighbor;
              next_best_g_score = g_score[neighbor];
            }
          }
          if (next_best_g_score ==
              numeric_limits<uint>::max())  // OK, now we consider going back!
            next_best = prev;
        }
      }
      prev = current;
      current = next_best;
      portion_path.push_back(current);
      steps_left--;
      this->prev_node = prev;
      this->current_node = current;
      continue;
    }
    // The second case: the next best node is occupied
    // get non - obstacles neighbors
    uint x = current.x;
    uint y = current.y;
    /*cout << "current: " << current << endl;
    cout << "prev: " << prev << endl;*/

    vector<Node> neighbors = {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
    uint current_best_g_score = g_score[current];
    uint next_best_g_score = numeric_limits<uint>::max();

    for (auto& neighbor : neighbors) {
      if (neighbor == next_best) continue;
      if (neighbor == prev) continue;

      if (g_score[neighbor] < next_best_g_score &&
          space_map[i].find(neighbor) ==
              space_map[i]
                  .end())  // find best gScore and it is not in space_map
      {
        next_best = neighbor;
        next_best_g_score = g_score[neighbor];
      }
    }
    // f_score never updated, no neighbor except came_from which is occupied.
    // the next best is to stay at current. ??? or going back?
    if (next_best_g_score == numeric_limits<uint>::max()) break;
    prev = current;
    current = next_best;
    portion_path.push_back(current);
    steps_left--;

    this->prev_node = prev;
    this->current_node = current;
  }
  while (steps_left--) {
    portion_path.push_back(current);
  }
}

void Agent::get_portion_path(std::list<Node>& p) {
  p.clear();
  for (auto& node : portion_path) p.push_back(node);
}

void Agent::set_current_node(const Node& n) {
  if (current_node != n) current_node = n;
}

Node Agent::get_current_node() const { return (current_node); }

Node Agent::get_prev_node() const { return (prev_node); }

uint Agent::get_path_length() const { return (path.size()); }

bool Agent::get_next_node(const Node& n, Node& next) {
  if (n == path.back()) {
    next = {std::numeric_limits<uint>::max(), std::numeric_limits<uint>::max()};
    return (false);
  }

  bool found = false;
  for (auto& node : path) {
    if (found) {
      next = node;
      return (true);
    }
    if (node == n) found = true;
  }
  return (false);
}

Node Agent::get_front_node() {
  return (path.empty() ? current_node : path.front());
}

void Agent::print_path() {
  for (const auto& node : path)
    if (node == path.back())
      cout << node << endl;
    else
      cout << node << "->";
}

void Agent::insert_path_after_front(const std::list<Node>& p) {
  auto current = path.begin();
  path.insert(++current, p.begin(), p.end());
}

void Agent::set_prev_node() { prev_node = get_prev_node(); }

void Agent::pop_front_node() {
  if (!path.empty()) path.pop_front();
}
