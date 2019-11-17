#pragma once

#include "Agent.hpp"
#include "Map.hpp"
#include "common.hpp"

using hash_map = std::unordered_map<Node, vector<Agent *>>;

inline uint manhattan_heuristic(const Node &a, const Node &b);

bool get_true_distance_heuristic(Agent &agent, Map &map,
                                 const Node &request_node);

void print_f_score(const Agent &agent, Map &map);

void print_g_score(const Agent &agent, Map &map);

void print_hash_map(const hash_map &hmap);

void print_path(const std::list<Node> path);

void print_neighbor_nodes(const vector<Node> v);

void print_agents_path(const vector<list<Node>> &v);

uint get_longest_agent_path(const vector<list<Node>> &v);

void print_conflict_agents(
    const vector<vector<Agent *>> &conflict_agents_pairs);

void print_space_map(const vector<unordered_map<Node, Agent *>> &space_map,
                     Map &map);

void print_agents_current_position(vector<Agent *> agents);

void update_space_map(vector<unordered_map<Node, Agent *>> &space_map,
                      Agent *agent);
