#pragma once
#include "Agent.hpp"

namespace Agent_tests {
bool check_get_name(const Agent& agent, char result) {
  return (agent.get_name() == result);
}
}  // namespace Agent_tests
