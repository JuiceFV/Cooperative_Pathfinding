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

/* set_portion_path
** here will be a code
*/

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
    cout << "Ti daun chi sho?! Ya doehal. Vo eblan na programiste" << endl;
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
}  //  TODO: fix the comment;

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
