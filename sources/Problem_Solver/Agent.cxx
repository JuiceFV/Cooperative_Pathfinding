#include "Agent.hpp"

char Agent::get_name() const { return (name); }

void Agent::set_start(const Node& s) {
  start = s;          // set the start node
  path.push_back(s);  // add the start node into the path
  current_node = s;   // set that we are on the start right now
}

Node Agent::get_start() const { return (start); }

void Agent::set_goal(const Node& g) { goal = g; }

Node Agent::get_goal() const { return (goal); }

void Agent::set_portion_path(
    const std::vector<std::unordered_map<Node, Agent*>>& space_map) {
  portion_path.clear();  // if there exist the path we're delete it.

  uint steps_left = WINDOW_SIZE;
  Node current = current_node;
  Node prev = current_node;
  Node next_best = current;  // it's a next node wich has the best score

  if (current == goal) {  // Hmm may be we are already at the goal?
    for (uint i = 0; i < WINDOW_SIZE; ++i) portion_path.push_back(goal);
    return;
  }
  for (auto i = 0; i < WINDOW_SIZE - 1; ++i) {
    if (current == goal) break;  // Did we find the goal? YEAH!!!

    if (came_from.find(current) == came_from.end())
      break;  // came from nowhere? How?

    next_best = came_from[current];  // Ok, let's take the child of the current

    /****************************************************************************
        First case: next best node is not occupied, but is it safe for
        me to go there? Let's fancy just part of the map (only one line):
        ___________________
        |0|1|2|3|4|5|6|7|8|	May be the agent want to out of the map.
       <-A| | | | | | | | | It's bad. Let's check this out.
    *****************************************************************************/
    if (&(space_map[i].find(next_best)) != &(space_map[i + 1].end())) {
      auto search1 = space_map[i].find(current);

      // Probably we already out of the map?
      if (search1 != space_map[i].end()) {
        // Ok, let's check if there another agent
        Agent* another_agent = search1->second;

        // Let's take a beside node. Can the second walk here?
        auto search2 = space_map[i - 1].find(next_best);

        // Fuck they are going to cllide
        if (search1->second == search2->second) {
          uint x = current.x;
          uint y = current.y;

          // Hold on! May be there are the nodes which are empty?
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
              next_best = neighbor;  // We found the best node where we can
                                     // go!!!
              next_best_g_score = g_score[neighbor];  // Let's set g(n)
            }
          }

          // Fuck this we didn' find any node where the agent could to go.
          if (next_best_g_score ==
              numeric_limits<uint>::max())  // OK, now we consider going back!
            next_best = prev;
        }
      }

      // Move! Let another agent go.
      prev = current;
      current = next_best;
      portion_path.push_back(current);
      steps_left--;
      this->prev_node = prev;
      this->current_node = current;
      continue;
    }

    /**********************************************************************************
        The second case: the next best node is occupied get non -
        obstacles neighbors.
    ***********************************************************************************/
    uint x = current.x;
    uint y = current.y;

    vector<Node> neighbors = {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
    uint current_best_g_score = g_score[current];
    uint next_best_g_score = numeric_limits<uint>::max();

    for (auto& neighbor : neighbors) {
      if (neighbor == next_best) continue;
      if (neighbor == prev) continue;

      if (g_score[neighbor] < next_best_g_score &&
          space_map[i].find(neighbor) ==
              space_map[i].end())  // find best g(n) and it is not in space_map
      {
        // as I had wrote before: "We found, where we can to go"
        next_best = neighbor;
        next_best_g_score = g_score[neighbor];
      }
    }
    /**************************************************************************
        f_score never updated, no neighbor except came_from which is occupied.
        The next best is to stay at current. ??? or going back?
    ***************************************************************************/
    if (next_best_g_score == numeric_limits<uint>::max()) break;

    // Still moving ...
    prev = current;
    current = next_best;
    portion_path.push_back(current);
    steps_left--;
    this->prev_node = prev;
    this->current_node = current;
  }

  /********************************************************************************
        We out of from cllision dection, for some reasons. Just fill the portion
        path with current node.
  *********************************************************************************/
  while (steps_left--) {
    portion_path.push_back(current);
  }
}

void Agent::get_portion_path(std::list<Node>& p) {
  p.clear();
  for (auto& node : portion_path) p.push_back(node);
}

Node Agent::get_current_node() const { return (current_node); }

#if defined(UNUSEFUL)
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

void Agent::set_current_node(const Node& n) {
  if (current_node != n) current_node = n;
}

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
#endif
