#include "../tests/tests.hpp"
#include "Agent.hpp"
#include "Map.hpp"
#include "Solver.hpp"
#include "common.hpp"

static void set_agent(Agent& agent, const Node start, const Node goal) {
  agent.set_start(start);
  agent.set_goal(goal);
}

static void add_agents_into_list(vector<Agent*>& agent_list, list<Agent>& list,
                                 Map& map) {
  for (auto& agent : list) {
    agent_list.push_back(&agent);
    if (!get_true_distance_heuristic(agent, map, agent.get_start()))
      std::cout << "Agent " << agent.get_name()
                << " failed at searching requested location!\n";
  }
}
int main() {
  Map map(8, 16, 20);  // 8 is WINDOW_SIZE
  map.print_map();
  cout << endl;

  std::vector<Agent*> agent_list;

  Agent a('A');
  set_agent(a, {1, 1, 0}, {8, 1, numeric_limits<uint>::max()});

  Agent b('B');
  set_agent(b, {8, 1, 0}, {1, 1, numeric_limits<uint>::max()});

  agent_list.push_back(&a);
  if (!get_true_distance_heuristic(a, map, a.get_start()))
    cout << "Agent " << a.get_name()
         << " failed at searching requested location!\n ";

  agent_list.push_back(&b);
  if (!get_true_distance_heuristic(b, map, b.get_start()))
    cout << "Agent " << b.get_name()
         << " failed at searching requested location!\n ";

  uint n_agents = agent_list.size();
  bool all_agents_find_goal = false;
  while (!all_agents_find_goal) {
    cout << ">>>>>>>>>>>>>>>>>    Do all agents reach destination?    "
            "<<<<<<<<<<<<<<<<\n";
    uint n_agents_at_goal = 0;
    for (auto i = 0; i < n_agents; ++i)
      if (agent_list[i]->get_current_node() == agent_list[i]->get_goal())
        n_agents_at_goal++;
    if (n_agents_at_goal == n_agents) {
      all_agents_find_goal = true;
      cout << "Success! All agents reached goal!\n";
      break;
    }

    vector<unordered_map<Node, Agent*>> space_time_map(WINDOW_SIZE);
    for (auto agent : agent_list) {
      agent->set_portion_path(space_time_map);
      update_space_map(space_time_map, agent);
    }
  }
#if defined(_MSC_VER) || defined(_WIN32)
  system("pause");
#endif
  return (0);
}
