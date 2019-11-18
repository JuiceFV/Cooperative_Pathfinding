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

#define WINDOW_SIZE 8
//#define UNUSEFUL

using namespace std;
using uint = unsigned int;

// Our basuc unit of map/path
typedef struct Node {
  uint x, y, f_score, g_score;

  bool operator==(const Node &n) const { return (x == n.x && y == n.y); }

  bool operator!=(const Node &n) const { return (x != n.x || y != n.y); }

  friend std::ostream &operator<<(std::ostream &out, const Node &n) {
    out << '[' << n.x << ", " << n.y << ']';
    return (out);
  }
} Node;

/*******************************************************
        We've created these structures for the accurate
        (needed) order in priority_queue
********************************************************/
typedef struct compare_node_f_score {
  bool operator()(const Node &a, const Node &b) const {
    return (a.f_score > b.f_score);
  }
} compare_node_f_score;

typedef struct compare_node_g_score {
  bool operator()(const Node &a, const Node &b) const {
    return (a.g_score > b.g_score);
  }
} compare_node_g_score;

/********************************************************************
        Because there is no the specialization for the my own
        struct Node. The unordered_map uses the std::hash, therefore
        we need to specialize the hash.
        You can find some information here:
        https://en.cppreference.com/w/cpp/utility/hash
*********************************************************************/
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

/**********************************************************************
        Because of the STL priority_queue doesn't has a "find" method.
        OK, let's specialize it.
***********************************************************************/
template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
class prior_queue : public std::priority_queue<T, Container, Compare> {
 public:
  typedef typename std::priority_queue<
      T, Container, Compare>::container_type::const_iterator const_iterator;
  bool find(const T &val) const {
    auto first = this->c.cbegin();
    auto last = this->c.cend();
    while (first != last) {
      if (*first == val) return (true);
      ++first;
    }
    return (false);
  }
};

/***************************************************************
        Priority queue specialized for gScore( lowest on top )
        gScore is the true distance from start to goal
****************************************************************/
using p_queue = prior_queue<Node, std::vector<Node>, compare_node_g_score>;
