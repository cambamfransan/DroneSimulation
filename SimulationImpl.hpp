#ifndef SIMULATION_IMPL_HPP
#define SIMULATION_IMPL_HPP

#include "State.hpp"

namespace simulation
{
  Coordinate getNewTarget(const State& state, const Drone& drone);

  Result runSimulation(Task task);

  void solveState(State& state);
}

#endif // !SIMULATION_IMPL_HPP
