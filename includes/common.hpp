#pragma once

#include <omp.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <unordered_map>
#include <utility>

#define WINDOW_SIZE 10

using namespace std;
using uint = unsigned int;

typedef struct Node {
  uint x, y, f_score, g_score;

  bool operator==(const Node &n) const { return (x == n.x && y == n.y); }

  bool operator!=(const Node &n) const { return (x != n.x || y != n.y); }

  friend std::ostream &operator<<(std::ostream &out, const Node &n) {
    out << '[' << n.x << ", " << n.y << ']';
    return (out);
  }
} Node;

// We're created these structures for the prior_queue

typedef struct compare_node_f_score {
  bool operator()(const Node &a, const Node &b) const {
    return (a.f_score > b.f_score);
  }
} compare_node_f_score;

// Because there is no the specialization for the my own
// struct Node. The unordered_map uses the std::hash, therefor
// we need to specialize the hash.
// We can find some information here:
// https://en.cppreference.com/w/cpp/utility/hash

namespace std {
template <>
struct hash<Node> {
  std::size_t operator()(const Node &n) const {
    std::size_t const h1 = std::hash<std::size_t>{}(n.x);
    std::size_t const h2 = std::hash<std::size_t>{}(n.y);
    return (h1 ^ (h2 << 1));
  }
};

}  // namespace std
