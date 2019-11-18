#pragma once

#include "common.hpp"

class Agent {
 public:
  // Public variables
  std::unordered_map<Node, Node> came_from;  // <Parent, Child>
  std::unordered_map<Node, unsigned int>
      g_score;  // distance between start and current node
  std::unordered_map<Node, unsigned int> f_score;     // g_score + heuristic
  std::unordered_map<Node, unsigned int> closed_set;  // explored nodes
  prior_queue<Node, std::vector<Node>, compare_node_f_score>
      open_set;  // lowest fscore on top()

  // Consructor. Just set the name and clear the agent's path
  Agent(const char n) : name(n) { path.clear(); };

  // Getters and Setters
  char get_name() const;          // get agent name
  void set_start(const Node &s);  // set start point of the agent
  Node get_start() const;         // get start point of the agent
  void set_goal(const Node &g);   // set goal of the agent
  Node get_goal() const;          // get goal of the agent
  void set_portion_path(const std::vector<std::unordered_map<Node, Agent *>>
                            &space_map);  // the main colision solver

  void get_portion_path(std::list<Node> &p);  // recording path into 'p'
  Node get_current_node() const;              // get_current_node

  /*************************************************************
        There I will put unuseful methods, that I used so far.
        You can uncomment the define of UNUSEFUL in common.hpp
  **************************************************************/
#if defined(UNUSEFUL)
  void set_whole_path();                          // set whole path using A*
  void get_path(std::list<Node> &p);              // path is recording into 'p'
  void set_current_node(const Node &n);           // recording node into 'n'
  uint get_path_length() const;                   // get length of the path
  Node get_front_node();                          // get last recorded node
  Node get_prev_node() const;                     // get previous node
  void set_prev_node();                           // set previous node
  bool get_next_node(const Node &n, Node &next);  // get next part of the path

  void print_path();  // print function

  void insert_path_after_front(const std::list<Node> &p);
  void pop_front_node();  // pop front part of(node) the path
#endif
 private:
  std::list<Node> path;          // the path of the agent
  std::list<Node> portion_path;  // The path of space_time_map
  Node start;                    // the start point
  Node goal;                     // the goal point
  Node prev_node;                // previous node
  Node current_node;             // current node (omg realy???)
  Node next_node;                // next possible move
  char name;  // the name of Agent it's just a letter 'A', 'B' etc.
};
