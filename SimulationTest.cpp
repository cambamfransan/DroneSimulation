#include "SimulationImpl.hpp"

namespace
{
  Coordinate getNewTarget(const State& state, const Drone& drone)
  {
    std::vector<std::pair<double, Coordinate>> dest;
    // find possible destinations
    for (auto&& t : state.m_targetDeck.left)
    {
      size_t dtt =
        std::abs(static_cast<int>(t.x) - static_cast<int>(drone.loc.x)) +
        std::abs(static_cast<int>(t.x) - static_cast<int>(drone.loc.x));
      size_t dth = std::abs(static_cast<int>(state.m_homeLocation.x) -
                            static_cast<int>(drone.loc.x)) +
                   std::abs(static_cast<int>(state.m_homeLocation.x) -
                            static_cast<int>(drone.loc.x));
      if (dtt + dth > drone.batteryLifeLeft) continue;

      dest.push_back(std::make_pair(drone.valueFunction(dtt), t));
    }
    if (dest.empty()) return state.m_homeLocation;

    std::sort(
      dest.begin(),
      dest.end(),
      [](const std::pair<double, Coordinate>& a,
         const std::pair<double, Coordinate>& b) { return a.first > a.first; });
    double lower = dest[0].first * (1 - state.m_diffPercentage);
    size_t i;
    for (i = 0; i < dest.size() && dest[i].first < lower; i++)
      ;
    dest.resize(i - 1);
    srand(time(NULL));
    return dest[rand() % dest.size()].second;
  } // namespace

} // namespace

Result simulation::runSimulation(Task task)
{
  Result result;
  for (size_t i = 0; i < task.m_times; i++)
  {
    State state(task);
    while (!state.m_done)
      solveState(state);
    result.m_results[i] = state.m_targetDeck.hit.size();
  }
  return result;
}

void simulation::solveState(State& state)
{
  size_t done(0);
  for (auto&& drone : state.m_drones)
  {
    if (!drone.batteryLifeLeft)
    {
      done++;
      continue;
    }
    // solveDrone

    // if at target or lost target, get new target
    // if ((drone.target.x == drone.loc.x && drone.target.y == drone.loc.y) ||
    if (!state.m_map[drone.target.x][drone.target.y])
    {
      drone.target = getNewTarget(state, drone);
    }

    if (drone.loc.x != drone.target.x)
      drone.loc.x += drone.loc.x > drone.target.x ? -1 : 1;
    else // (drone.loc.y != drone.target.y)
      drone.loc.y += drone.loc.y > drone.target.y ? -1 : 1;
    drone.batteryLifeLeft--;
  }

  if (done == state.m_drones.size()) state.m_done = true;
}
