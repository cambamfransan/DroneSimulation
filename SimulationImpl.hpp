#ifndef SIMULATION_IMPL_HPP
#define SIMULATION_IMPL_HPP

#include "State.hpp"

namespace simulation
{
  Result runSimulation(Task task);

  void solveState(State& state);
}

#endif // !SIMULATION_IMPL_HPP
