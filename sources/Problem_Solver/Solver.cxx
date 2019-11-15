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

  // TODO: continue
}
