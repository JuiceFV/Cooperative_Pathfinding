#include <iostream>

#include "common.hpp"

static void set_node(Node &n, uint x, uint y) {
  n.x = x;
  n.y = y;
}

int main() {
  Node a, b;
  set_node(a, 1, 2);
  set_node(b, 1, 2);
  std::cout << "a: " << a << std::endl << "b: " << b << endl;
  a == b ? std::cout << "a equal b" : std::cout << "a does not equal b";
  std::cout << std::endl;
  std::cout << std::endl;
  set_node(b, 5, 6);
  std::cout << "a: " << a << std::endl << "b: " << b << endl;
  a != b ? std::cout << "a does not equal b" : std::cout << "a equal b";
  std::cout << std::endl;
#if defined(_MSC_VER) || defined(_WIN32)
  system("pause");
#endif
  return (0);
}
