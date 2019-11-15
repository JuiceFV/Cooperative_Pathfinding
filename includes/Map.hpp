#pragma once

#include "Agent.hpp"
#include "common.hpp"

class Map {
  void allocate_map();
  void allocate_map(uint obstacle_percentage);

 public:
  Map(uint w, uint h);
  Map(uint w, uint h, uint obstacle_percentage);
  Map(const Map& map);

  uint get_height() const;
  uint get_width() const;
  uint get_square() const;

  char** get_map_data() const;

  void set_obstacle(const uint w, const uint h);
  void set_obstacle(const vector<pair<uint, uint>>& p);
  void get_obstacles(unordered_map<Node, char>& m);
  bool is_obstacle(const Node& n);

  void get_neighbors(const Node& n, vector<Node>& Neighbors);
  void get_clean_neighbors(const Node& node, vector<Node>& nodes);

  void update_map(const vector<Agent>& agent_list);

  void print_map();
  void print_map_with_agents(const unordered_map<Node, Agent*>& map);
  void print_agent_path(const Agent& agent, const vector<Node>& path);
  // void print_time_space_map(const uint& time);

  ~Map();

 private:
  uint width, height;
  uint square;
  uint obstacles_percent;
  char** map_data;
  unordered_map<Node, char> obstacles;
};
