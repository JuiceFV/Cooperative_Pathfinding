#include "../tests/tests.hpp"
#include "Agent.hpp"
#include "common.hpp"

static void set_node(Node& n, uint x, uint y) {
  n.x = x;
  n.y = y;
}

static void print_list(list<int> l) {
  for (auto& i : l) cout << i << ' ';
  cout << endl;
}
int main() {
 
#if defined(_MSC_VER) || defined(_WIN32)
  system("pause");
#endif
  return (0);
}
