#include "Agent.hpp"
#include "Map.hpp"
#include "Solver.hpp"
#include "common.hpp"

// A simple function that configure the agent
static Agent set_agent(char name, const Node start, const Node goal) {
  Agent agent(name);       // set agnet's name
  agent.set_start(start);  // set agent's start position and push it in the path
  agent.set_goal(goal);    // set goal position
  return (agent);
}

int main() {
  /********************************************************
       We have created a map with width = 8
       and height = 16. If we'ill add the borders
       the width and height will be bigger on 2
       consequently the square = width * height
       square_width_borders = (width + 2) * (height + 2)
  *********************************************************/
  Map map(8, 16);

  /****************************************************************************
       If you wish to create the map with obstacles, just uncomment
        following code.It means that map will 8 * 16 and with 20% of obstacles
  *****************************************************************************/
  // Map map(8, 20, 20);

  /****************************************************************************
        OK, now let's create few agents (robots/cars, whatever you want)
        For instance, let's consider 2 agents, such that the first has name
        'A', the start point is [1, 1] and finish is [8, 1], and second
        has name 'B' with the start [8, 1] and finish [1, 1].
        It's will look like that:
        _____________________
        |#|#|#|#|#|#|#|#|#|#|	Let's consider this map. The square of the map
        |#|A|_|_|_|_|_|_|B|#|	is equal 8 * 16 = 128.
        |#|_|_|_|_|_|_|_|_|#|	A:					B:
        |#|_|_|_|_|_|_|_|_|#|	{					{
        |#|_|_|_|_|_|_|_|_|#|		name: 'A'			name: 'B;
        |#|_|_|_|_|_|_|_|_|#|		start: [1,1]		start: [8, 1]
        |#|_|_|_|_|_|_|_|_|#|		goal:  [8,1]		goal: [1, 1]
        |#|_|_|_|_|_|_|_|_|#|	}					}
        |#|_|_|_|_|_|_|_|_|#|	Yes, they just want to swap places. Of course,
        |#|_|_|_|_|_|_|_|_|#|	the shortest path is a straight path. But they
        |#|_|_|_|_|_|_|_|_|#|	will collide if they both will go straight.
        |#|_|_|_|_|_|_|_|_|#|	Hmm, so what we gonna do???
        |#|_|_|_|_|_|_|_|_|#|	Ok, let's figgure it out!!!
        |#|_|_|_|_|_|_|_|_|#|
        |#|_|_|_|_|_|_|_|_|#|
        |#|_|_|_|_|_|_|_|_|#|
        |#|#|#|#|#|#|#|#|#|#|

   ****************************************************************************/
  Agent a = set_agent('A', {1, 1, 0}, {8, 1, numeric_limits<uint>::max()});
  Agent b = set_agent('B', {8, 1, 0}, {1, 1, numeric_limits<uint>::max()});

  /**********************************************************************
        There we created a list of the agents, just for comfortable use.
        { agent_list = [&a, &b] | a, b belong to Agent*}
  ***********************************************************************/
  vector<Agent*> agent_list;

  /******************************************************************************
        Ok, now let's define a lambda function. Why? Because we use this
        function only once and only with essenational list of arguments, so why
        not? We creating a temp_list, from where they will poped. It's
        just temporary. And filling the agent_list, using lambda.
  *******************************************************************************/
  list<Agent> temp_list = {a, b};
  auto list_filling = [&agent_list, &map](list<Agent>& list) {
    for (auto& agent : list) {
      agent_list.push_back(&agent);

      // if the agent didn't find the distanation.
      if (!get_true_distance_heuristic(agent, map, agent.get_start())) {
        cerr << "Agent " << agent.get_name()
             << " failed at searching requested location!\n";
        exit(getchar());
      }
    }
  };
  list_filling(temp_list);

  /*******************************************************************************
        Set some variables:
        n_agents: it's number of agents, that there are exists on the map
        all_agents_find_goal: indecator that everyone has reached their own goal
  *******************************************************************************/
  uint n_agents = agent_list.size();
  bool all_agents_find_goal = false;

  /****************************************************************************
        Start main loop, it will be work until that moment, whilst all agents
        won't reach the goals.
  *****************************************************************************/
  while (!all_agents_find_goal) {
    uint n_agents_at_goal = 0;  // number of agents, which reached the goal

    for (auto i = 0; i < n_agents; ++i)
      if (agent_list[i]->get_current_node() == agent_list[i]->get_goal())
        n_agents_at_goal++;  // there we just count how much robots are at the
                             // goal

    if (n_agents_at_goal == n_agents) {  // Yeah! We got it!!! And we don't need
                                         // to find the goals anymore
      all_agents_find_goal = true;
      cout << "Success! All agents reached goal!\n";
      break;
    }

    /**************************************************************************************************
			There we starting the
			main computational action of the program Let me explain a little
			bit.

			Time is not time like
			min or sec. Time is unit to represent a frame, a step for all
			agents. below, I used a vector. space_time_map[0] means at the begining.
			space_time_map[7] means after 7 steps, at step 8 or the end of current
			window, where are the agents located on the map. say if at time 5, 'A' is
			located at node (3,4). how to capture 'A'? space_time_map[4][{3,4}] ; I hope
			you got the idea, the syntax may be wrong. notice that, one node, one agent,
			it is not allowed to have multiple agents sitting at one node. Now I hope
			that you know what is space_time_map, and what I mean by time.
    ***************************************************************************************************/
    vector<unordered_map<Node, Agent*>> space_time_map(WINDOW_SIZE);
    for (auto agent : agent_list) {
      agent->set_portion_path(space_time_map);
      update_space_map(space_time_map, agent);
    }
    print_space_map(space_time_map, map);
  }

#if defined(_MSC_VER) || defined(_WIN32)
  system("pause");
#endif
  return (0);
}
