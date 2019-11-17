#include "Solver.hpp"

/*****Manhattan heuristic function*****************************************
You can looking for the more information here:
http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
The breif description of the Manhattan Heuristic Distance:
It's a distance between start and goal without taking into account
the obstacles.
****************************************************************************/
inline uint manhattan_heuristic(const Node& a, const Node& b) {
  return (10 * (std::abs(static_cast<int>(a.x - b.x)) +
                std::abs(static_cast<int>(a.y - b.y))));
}

/*****Reverse Resumable A Star************************************************
Bofore we start please read the description of the
basic A* and you will comprehend what is happening
here. Here few links:
[1] https://en.wikipedia.org/wiki/A*_search_algorithm
[2] http://theory.stanford.edu/~amitp/GameProgramming/AStarComparison.html

RRA* is a Backwards Search ignoring other agents the g(n) value
(measured distance to goal) is the true distance heuristic value. if g(n)
value of requested node is not known, set goal at requested_node.
******************************************************************************/
bool get_true_distance_heuristic(Agent& agent, Map& map,
                                 const Node& request_node) {
  bool goal_found = false;  // the indecator, is the goal found

  /*******************************************************
        Hmmm, what if we were fucked up and passed the
        obstacle as request_node. Ok, nevermind, I will
        check it anyway.
  ********************************************************/
  if (map.is_obstacle(request_node)) {
    cout << "RRA*: Requested node is an obstacle.\n";
    agent.closed_set.insert({request_node, numeric_limits<uint>::max()});
    return (false);
  }

  // Simple logic. IT'S RRA* (REVERSE!!!)
  Node start = agent.get_goal();  // so we're going from finish
  Node goal = request_node;       // to the start.

  /*************************************************
       1) Initially, only the start node is known.
       2) The distance between start and start is 0
       3) Heuristic distance between start and goal
   *************************************************/
  agent.open_set.push(start);
  agent.g_score.insert({start, 0});
  agent.f_score.insert({start, manhattan_heuristic(start, goal)});

  // The Search start ...
  while (!agent.open_set.empty()) {
    // Let's check the top node
    Node current = agent.open_set.top();
    agent.open_set.pop();

    if (current == goal) {  // Probably it's goal
      goal_found = true;
    }

    // while we don't forget let's add this node into explored
    agent.closed_set.insert({current, agent.f_score[current]});

    /**********************************************************************
    Explore the neighbors around the current node. And we don't care what
    exactly the neighbor (Either obstacle or empty or other agent).
    **********************************************************************/
    vector<Node> neighbors;
    map.get_neighbors(current, neighbors);

    for (Node& neighbor : neighbors) {
      // if neighbors not exist, create new nodes
      if (agent.came_from.find(neighbor) == agent.came_from.end()) {
        agent.came_from.insert(make_pair(neighbor, current));
        agent.g_score.insert(make_pair(neighbor, numeric_limits<uint>::max()));
        agent.f_score.insert(make_pair(neighbor, numeric_limits<uint>::max()));
      }
      // Ignore the neighbor which is already evaluated
      if (agent.closed_set.find(neighbor) != agent.closed_set.end()) continue;

      // if neighbor node is obstacle, paste to closed_set and continue.
      if (map.is_obstacle(neighbor)) {
        agent.closed_set.insert({neighbor, numeric_limits<uint>::max()});
        continue;
      }

      /****************************************************
              In future I will add the diagonal moves.
              Thus, the horizontal/vertical weight is 10
              diagonal weight is 14
      ****************************************************/
      uint dist_neighbor = abs((int)(neighbor.x - current.x)) +
                                       abs((int)(neighbor.y - current.y)) ==
                                   2
                               ? 14
                               : 10;
      uint tentative_g_score = agent.g_score[current] + dist_neighbor;
      if (tentative_g_score >= agent.g_score[neighbor]) continue;

      agent.came_from[neighbor] = current;
      agent.g_score[neighbor] = tentative_g_score;

      /************************************************************************
        If goal is already found, manhattan_heuristic is inaccurate to
        measure f_score. because manhanttan heuristic ignore obstacles. If goal
        is found, we simply add 20, why? for example, if we know node A's
        f_score and node B is A's neighbor, and if A is the only parent of B,
        then the cost of going from A to B is 10, so B_f_score = A_f_sore + 10;
        in order for B to reach goal, it must go back to A, so add 10 to
        B_f_score again, total is 20;
      ***********************************************************************/
      if (!goal_found) {
        agent.f_score[neighbor] =
            tentative_g_score + manhattan_heuristic(neighbor, goal);
        neighbor.f_score =
            tentative_g_score + manhattan_heuristic(neighbor, goal);
      } else {
        uint new_f_score = agent.f_score[current] + 20;
        agent.f_score[neighbor] = new_f_score;
        neighbor.f_score = new_f_score;
      }
      agent.open_set.push(neighbor);
    }
  }
  return ((goal_found) ? true : false);
}

/********************************************************************/
/* The following functions are only helpful. I won't to explain it! */
/********************************************************************/

void print_f_score(const Agent& agent, Map& map) {
  uint width = map.get_width();
  uint height = map.get_height();
  for (uint w = 0; w < width; ++w) cout << "#\t";
  cout << endl;
  for (uint h = 1; h < height - 1; ++h) {
    cout << "#\t";
    for (uint w = 1; w < width - 1; ++w) {
      Node node{w, h};
      if (map.is_obstacle(node)) {
        cout << "#\t";
        continue;
      }
      auto search = agent.closed_set.find(node);
      if (search != agent.closed_set.end()) {
        if (search->second == numeric_limits<uint>::max())
          cout << "INF\t";
        else
          cout << search->second << "\t";
      } else
        cout << "0\t";
    }
    cout << "#\n";
  }
  for (uint w = 0; w < width; ++w) cout << "#\t";
  cout << endl;
}

void print_g_score(const Agent& agent, Map& map) {
  uint width = map.get_width();
  uint height = map.get_height();
  for (uint w = 0; w < width; ++w) cout << "#\t";
  cout << endl;
  for (uint h = 1; h < height - 1; ++h) {
    cout << "#\t";
    for (uint w = 1; w < width - 1; ++w) {
      Node node{w, h};
      if (map.is_obstacle(node)) {
        cout << "#\t";
        continue;
      }
      auto search = agent.g_score.find(node);
      if (search != agent.g_score.end()) {
        if (search->second == numeric_limits<uint>::max())
          cout << "#\t";
        else
          cout << search->second << "\t";
      } else
        cout << "0\t";
    }
    cout << "#\n";
  }
  for (uint w = 0; w < width; ++w) cout << "#\t";
  cout << endl;
}

void print_hash_map(const hash_map& hmap) {
  for (const auto& i : hmap) {
    cout << "Node: " << i.first << " Agent: ";
    for (const auto& j : i.second) cout << j->get_name() << " ";
    cout << endl;
  }
  cout << endl;
}

void print_path(const list<Node> path) {
  for (const auto& i : path)
    if (i == path.back())
      cout << i << endl;
    else
      cout << i << "->";
}

void print_neighbor_nodes(const vector<Node> v) {
  for (const auto& i : v) cout << i << " ";
  cout << endl;
}

void print_agents_path(const vector<list<Node>>& v) {
  int n = 0;
  for (const auto& i : v) {
    cout << "Agent " << n++ << ": ";
    print_path(i);
  }
}

uint get_longest_agent_path(const vector<list<Node>>& v) {
  uint max = 0;
  for (const auto& i : v) {
    if (max < i.size()) max = i.size();
  }
  return max;
}

void print_conflict_agents(
    const vector<vector<Agent*>>& conflict_agents_pairs) {
  cout << "The pair of the agents, who have conflicts:\n";
  for (const auto& agents : conflict_agents_pairs) {
    cout << "[ ";
    for (const auto& i : agents) cout << i->get_name() << " ";
    cout << " ]\n";
  }
}

void print_space_map(const vector<unordered_map<Node, Agent*>>& space_map,
                     Map& map) {
  for (auto i = 0; i < WINDOW_SIZE; i++) {
    cout << "\nspace_map at time [" << i << "]\n";
    map.print_map_with_agents(space_map[i]);
    _sleep(1000);
    system("cls");
  }
}

void print_agents_current_position(vector<Agent*> agents) {
  cout << "agents current position:\n";
  for (const auto& i : agents)
    cout << i->get_name() << ": " << i->get_current_node() << endl;
  cout << endl;
}

void update_space_map(vector<unordered_map<Node, Agent*>>& space_map,
                      Agent* agent) {
  list<Node> l;
  agent->get_portion_path(l);
  for (auto i = 0; i < WINDOW_SIZE; ++i) {
    space_map[i][l.front()] = agent;
    l.pop_front();
  }
}
