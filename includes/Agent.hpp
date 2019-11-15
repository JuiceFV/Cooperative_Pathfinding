#pragma once

#include "common.hpp"

class Agent {
 public:
  // Public variables
  std::unordered_map<Node, Node> came_from;  // <Parent, Child>
  std::unordered_map<Node, unsigned int> g_score;
  std::unordered_map<Node, unsigned int> f_score;
  std::unordered_map<Node, unsigned int> closed_set;
  prior_queue<Node, std::vector<Node>, compare_node_f_score>
      open_set;  // lowest fscore on top()

  // Consructor
  Agent(const char n) : name(n) { path.clear(); };

  // Getters and Setters
  char get_name() const;
  void set_start(const Node &s);
  Node get_start() const;
  void set_goal(const Node &g);
  Node get_goal() const;
  void set_whole_path();
  void get_path(std::list<Node> &p);
  void set_portion_path(
      const std::vector<std::unordered_map<Node, Agent *>> &space_map);
  void get_portion_path(std::list<Node> &p);
  void set_current_node(const Node &n);
  Node get_current_node() const;
  Node get_prev_node() const;
  uint get_path_length() const;
  bool get_next_node(const Node &n, Node &next);
  Node get_front_node();

  // print function
  void print_path();

  // the functions which working with a path
  void insert_path_after_front(const std::list<Node> &p);
  void pop_front_node();
  void set_prev_node();

 private:
  std::list<Node> path;
  std::list<Node> portion_path;  // The path of space_time_map
  Node start;
  Node goal;
  Node prev_node;
  Node current_node;
  Node next_node;
  char name;  // the name of Agent it's just a letter 'A', 'B' etc.
};
