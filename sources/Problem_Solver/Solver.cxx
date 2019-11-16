#include "Solver.hpp"

inline uint manhattan_heuristic(const Node& a, const Node& b) {
  return (10 * (std::abs(static_cast<int>(a.x - b.x)) +
                std::abs(static_cast<int>(a.y - b.y))));
}

bool a_star_search(Agent& agent, Map& map) {
  // empty agent's closedSet
  if (!agent.closed_set.empty()) agent.closed_set.clear();

  if (!agent.open_set.empty()) cout << "open_set is is not empty, error\n";
  Node start = agent.get_start();
  Node goal = agent.get_goal();
  // Initially, only the start node is known.
  agent.open_set.push(start);
  // The cost of going from start to start is zero.
  agent.g_score.insert({start, 0});
  // For the first node, that value is completely heuristic.
  agent.f_score.insert({start, manhattan_heuristic(start, goal)});
  while (!agent.open_set.empty()) {
    Node current = agent.open_set.top();
    agent.open_set.pop();

    if (current == goal) return (true);

    agent.closed_set.insert(
        {current, agent.f_score[current]});  // change closedSet to vector
    vector<Node> neighbors;
    map.get_neighbors(current, neighbors);

    for (Node& neighbor : neighbors) {
      // insert new nodes
      if (agent.came_from.find(neighbor) == agent.came_from.end()) {
        agent.came_from.insert(make_pair(neighbor, current));
        agent.g_score.insert(make_pair(neighbor, numeric_limits<uint>::max()));
        agent.f_score.insert(make_pair(neighbor, numeric_limits<uint>::max()));
      }
      if (agent.closed_set.find(neighbor) != agent.closed_set.end())
        continue;  // Ignore the neighbor which is already evaluated

      if (map.is_obstacle(
              neighbor)) {  // if neighbor is obstacles, add to closedSet.
        agent.closed_set.insert(
            {neighbor,
             numeric_limits<uint>::max()});  // fscore is set to be max.
        continue;
      }

      // The distance from start to a neighbor, diagonal 14, vertical or
      // horizontal 10.
      uint dist_neighbor = abs((int)(neighbor.x - current.x)) +
                                       abs((int)(neighbor.y - current.y)) ==
                                   2
                               ? 14
                               : 10;

      uint tentative_g_score = agent.g_score[current] + dist_neighbor;

      if (tentative_g_score >= agent.g_score[neighbor]) continue;

      agent.came_from[neighbor] = current;
      agent.g_score[neighbor] = tentative_g_score;
      agent.f_score[neighbor] =
          tentative_g_score + manhattan_heuristic(neighbor, goal);
      neighbor.f_score =
          tentative_g_score + manhattan_heuristic(neighbor, goal);
      agent.open_set.push(neighbor);
    }
  }
  return (false);
}

//   reverse resumable A* (Backwards Search ignoring other agents)
//   the g value (measured distance to goal) is the true distance heuristic
//   value. if g value of requested node is not known, set goal at
//   requestedNode.

bool get_true_distance_heuristic(Agent& agent, Map& map,
                                 const Node& request_node) {
  bool goal_found = false;

  // check requested Node
  if (map.is_obstacle(request_node)) {
    cout << "RRA*: Requested node is an obstacle.\n";
    agent.closed_set.insert({request_node, numeric_limits<uint>::max()});
    return (false);
  }

  // start position is the goal position
  Node start = agent.get_goal();
  Node goal = request_node;

  // Initially, only the start node is known.
  agent.open_set.push(start);
  agent.g_score.insert({start, 0});
  agent.f_score.insert({start, manhattan_heuristic(start, goal)});
  while (!agent.open_set.empty()) {
    Node current = agent.open_set.top();
    agent.open_set.pop();
    if (current == goal) {
      goal_found = true;
    }
    agent.closed_set.insert(
        {current, agent.f_score[current]});  // change closedSet to vector.

    vector<Node> neighbors;
    map.get_neighbors(current, neighbors);
    for (Node& neighbor : neighbors) {
      // if neighbors not exist, creat new nodes
      if (agent.came_from.find(neighbor) == agent.came_from.end()) {
        agent.came_from.insert(make_pair(neighbor, current));
        agent.g_score.insert(make_pair(neighbor, numeric_limits<uint>::max()));
        agent.f_score.insert(make_pair(neighbor, numeric_limits<uint>::max()));
      }

      if (agent.closed_set.find(neighbor) != agent.closed_set.end()) continue;

      if (map.is_obstacle(neighbor)) {
        agent.closed_set.insert({neighbor, numeric_limits<uint>::max()});
        continue;
      }

      uint dist_neighbor = abs((int)(neighbor.x - current.x)) +
                                       abs((int)(neighbor.y - current.y)) ==
                                   2
                               ? 14
                               : 10;
      uint tentative_g_score = agent.g_score[current] + dist_neighbor;
      if (tentative_g_score >= agent.g_score[neighbor]) continue;

      agent.came_from[neighbor] = current;
      agent.g_score[neighbor] = tentative_g_score;
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
    }
    return (true);
  }
}

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
    cout << endl;
  }
}

void print_agents_current_position(vector<Agent*> agents) {
  cout << "agents current position:\n";
  for (const auto& i : agents)
    cout << i->get_name() << ": " << i->get_current_node() << endl;
  cout << endl;
}

// check face to face collisions, each colliion involves two agents,
// where agent1 walking from A to B, agent2 walking from B to A
bool check_collision_type2(const hash_map& hmap_t0, hash_map& hmap_t1,
                           vector<vector<Node>>& collision_nodes_pairs) {
  bool conflict_exist = false;

  for (const auto& hash0 : hmap_t0) {
    char first_agent_name =
        hash0.second[0]
            ->get_name();  // First agent is walking from node0 to node0_next
    Node first_agent_node0 = hash0.first;
    Node first_agent_node1;
    if (!hash0.second[0]->get_next_node(first_agent_node0, first_agent_node1)) {
      cout << "check_collision_type2: " << first_agent_name
           << ": there is no node after " << first_agent_node0 << endl;
      continue;
    }
    cout << first_agent_name << " is walking from " << first_agent_node0
         << " to " << first_agent_node1 << endl;

    for (const auto& hash1 : hmap_t1) {
      char second_agent_name = hash1.second[0]->get_name();
      if (second_agent_name == first_agent_name)  // if itself - skip
        continue;

      Node second_agent_node1 = hash1.first;

      if (second_agent_node1 == first_agent_node0) {
        Node second_agent_node0 = hash1.second[0]->get_current_node();
        if (second_agent_node0 == first_agent_node1) {  // voila, c'est tout!
          cout << "We should to stop " << second_agent_name << " walking from "
               << second_agent_node0 << " to " << second_agent_node1 << endl;
          vector<Node> nodes_pair{first_agent_node0, first_agent_node1};
          collision_nodes_pairs.push_back(nodes_pair);
          conflict_exist = true;
          hmap_t1.erase(second_agent_node0);
          hmap_t1.erase(second_agent_node1);
          if (hmap_t1.empty()) break;
        }
      }
    }
  }
  return (conflict_exist);
}

// both agents will move
void fix_pair(hash_map& hmap_t0, hash_map& hmap_t1,
              Node first_agent_current_node, Node& second_agent_current_node,
              Map& m) {
  bool collision_fixed = false;

  Agent* first_agent = hmap_t0[first_agent_current_node][0];
  Agent* second_agent = hmap_t0[second_agent_current_node][0];
  // g_score is true distance to goal
  uint first_agent_current_node_g_score =
      first_agent->g_score[first_agent_current_node];
  uint second_agent_current_node_g_score =
      second_agent->g_score[second_agent_current_node];

  Node first_agent_prev_node = first_agent->get_prev_node();
  Node second_agent_prev_node = second_agent->get_prev_node();
  // compare two gScore, if it's of the first agent is lower,
  // means that the first agent is closer to goal, the second one try to make
  // way for the first one
  cout << first_agent->get_name() << ": " << first_agent_current_node
       << " g_score: " << first_agent_current_node_g_score
       << " previous node: " << first_agent_prev_node << endl;
  cout << second_agent->get_name() << ": " << second_agent_current_node
       << " g_score: " << second_agent_current_node_g_score
       << " previous node: " << second_agent_prev_node << endl;
  vector<Node> first_agent_possible_move, second_agent_possible_move;
  m.get_clean_neighbors(first_agent_current_node, first_agent_possible_move);
  m.get_clean_neighbors(second_agent_current_node, second_agent_possible_move);
  // lowest g_score on top
  p_queue first_agent_g_score_queue;
  p_queue second_agent_g_score_queue;
  cout << "Agent " << first_agent->get_name()
       << " ----possible next node g_score:\n";

  for (auto& first_agent_next_possible_node : first_agent_possible_move) {
    if (first_agent_next_possible_node == second_agent_current_node)
      continue;  // exclude its original node.
    first_agent_next_possible_node.g_score =
        first_agent->g_score[first_agent_next_possible_node];
    cout << first_agent_next_possible_node << " = "
         << first_agent->g_score[first_agent_next_possible_node] << endl;
    first_agent_g_score_queue.push(first_agent_next_possible_node);
  }
  cout << "Agent " << second_agent->get_name()
       << " ----possible next node g_score:\n";

  for (auto& second_agent_next_possible_node : second_agent_possible_move) {
    if (second_agent_next_possible_node == first_agent_current_node)
      continue;  // exclude its original next node.
    second_agent_next_possible_node.g_score =
        second_agent->g_score[second_agent_next_possible_node];
    cout << second_agent_next_possible_node << " = "
         << second_agent->g_score[second_agent_next_possible_node] << endl;
    second_agent_g_score_queue.push(second_agent_next_possible_node);
  }
  // case 1: one agent keep its path, the other changes. this is the best case!
  if (first_agent_current_node_g_score <=
      second_agent_current_node_g_score)  // check second agent queue i.e. agent
                                          // A
  {
    cout << "Agent " << second_agent->get_name() << " g_score checking ... "
         << endl;
    p_queue q2 = second_agent_g_score_queue;
    while (!q2.empty()) {
      Node second_agent_next_node = q2.top();
      if (second_agent_next_node ==
          second_agent_prev_node) {  // for now, we dont consider going back.
        q2.pop();
        continue;
      }

      cout << second_agent_next_node
           << " g_score = " << second_agent->g_score[second_agent_next_node]
           << endl;
      // we hope this is the next node for second agent.
      // but we first check if this node is gona taken by others.
      if (hmap_t1.find(second_agent_next_node) != hmap_t1.end())
        continue;  // meaning this node will be occupied by other agents

      cout << "find fix! " << second_agent_next_node << endl;
      list<Node> l2 = {second_agent_next_node, second_agent_current_node};
      cout << "inserting path to front ...";
      print_path(l2);
      second_agent->insert_path_after_front(
          l2);  // the second agent will walk back to its original path
      collision_fixed = true;
      break;
    }
  }
  if (!collision_fixed) {
    // The first one changes routes, the second one stays on course., check the
    // first agent queue. i.e. agent B
    cout << "Agent " << first_agent->get_name() << " g_score checking ... "
         << endl;
    p_queue q1 = first_agent_g_score_queue;
    while (!q1.empty()) {
      Node first_agent_next_node = q1.top();
      if (first_agent_next_node == first_agent_prev_node) {
        q1.pop();
        continue;
      }

      cout << first_agent_next_node
           << " g_score = " << first_agent->g_score[first_agent_next_node]
           << endl;
      if (hmap_t1.find(first_agent_next_node) != hmap_t1.end()) continue;
      cout << "find fix!" << first_agent_next_node << endl;
      // now we got a free node, no other agents are gona take it, so a1 will go
      // to that node temporarily.
      list<Node> l1 = {
          first_agent_next_node,
          first_agent_current_node};  // we assume the first agent will walk
                                      // back. if there is a collision, we deal
                                      // with it later...
      cout << "inserting path to front ...";
      print_path(l1);
      first_agent->insert_path_after_front(l1);
      cout << "new path is\n";
      first_agent->print_path();
      collision_fixed = true;
      break;
    }
  }
  if (collision_fixed)
    return;
  else
    cout << "type 2 collision not fixed!\n";
}

void fix_agents(hash_map& hmap_t0, hash_map& hmap_t1,
                vector<vector<Node>>& collision_nodes_pairs, Map& m) {
  cout << "Now we gonna fix the agents\n";
  uint nodes_pairs = collision_nodes_pairs.size();
  cout << "nodes_pairs = " << nodes_pairs << endl;
  cout << endl << endl;

  for (auto i = 0; i < nodes_pairs; i++) {
    Node node1, node2;
    node1 = collision_nodes_pairs[i][0];
    node2 = collision_nodes_pairs[i][1];

    cout << "node1: " << node1 << ", node2: " << node2 << endl;
    fix_pair(hmap_t0, hmap_t1, node1, node2, m);
  }
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
