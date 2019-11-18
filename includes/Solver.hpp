#pragma once

#include "Agent.hpp"
#include "Map.hpp"
#include "common.hpp"

// The reservation table
using hash_map = std::unordered_map<Node, vector<Agent *>>;

// The distance between node and goal
inline uint manhattan_heuristic(const Node &a, const Node &b);

// In other words it's RRA*
bool get_true_distance_heuristic(Agent &agent, Map &map,
                                 const Node &request_node);

// print our map on the stdout
void print_space_map(const vector<unordered_map<Node, Agent *>> &space_map,
                     Map &map);

// update our map
void update_space_map(vector<unordered_map<Node, Agent *>> &space_map,
                      Agent *agent);

#if defined(UNUSEFUL)
void print_f_score(const Agent &agent, Map &map);

void print_g_score(const Agent &agent, Map &map);

void print_hash_map(const hash_map &hmap);

void print_path(const std::list<Node> path);

void print_neighbor_nodes(const vector<Node> v);

void print_agents_path(const vector<list<Node>> &v);

uint get_longest_agent_path(const vector<list<Node>> &v);

void print_conflict_agents(
    const vector<vector<Agent *>> &conflict_agents_pairs);

void print_agents_current_position(vector<Agent *> agents);

bool check_collision_type2(const hash_map &hmap_t0, hash_map &hmap_t1,
                           vector<vector<Node>> &collision_nodes_pairs);

void fix_pair(hash_map &hmap_t0, hash_map &hmap_t1,
              Node first_agent_current_node, Node &second_agent_current_node,
              Map &m);

void fix_agents(hash_map &hmap_t0, hash_map &hmap_t1,
                vector<vector<Node>> &collision_nodes_pairs, Map &m);

bool a_star_search(Agent &agent, Map &map);
#endif
