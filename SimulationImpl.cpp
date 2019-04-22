#include "SimulationImpl.hpp"

#include <iostream>

Coordinate simulation::getNewTarget(const State& state, const Drone& drone)
{
  std::vector<std::pair<double, Coordinate>> dest;
  // find possible destinations
  for (auto&& t : state.m_targetDeck.left)
  {
    size_t dtt = // drone to target
      std::abs(static_cast<int>(t.x) - static_cast<int>(drone.loc.x)) +
      std::abs(static_cast<int>(t.y) - static_cast<int>(drone.loc.y));
    size_t tth = // target to home
      std::abs(static_cast<int>(state.m_homeLocation.x) - 
                          static_cast<int>(t.x)) +
                 std::abs(static_cast<int>(state.m_homeLocation.y) -
                          static_cast<int>(t.y));
    if (dtt + tth > drone.batteryLifeLeft) continue;

    if (!dtt || !tth) return t;

    if (!tth && !dtt) return t;
    auto utility =
      drone.valueFunction(static_cast<double>(tth)) / static_cast<double>(dtt);
    dest.push_back({utility, t});
  }
  if (dest.empty()) return state.m_homeLocation;

  std::sort(
    dest.begin(),
    dest.end(),
    [](const std::pair<double, Coordinate>& a,
       const std::pair<double, Coordinate>& b) { return a.first > b.first; });
  double lower = dest[0].first * (1 - state.m_diffPercentage);
  size_t i;
  for (i = 0; i < dest.size() && dest[i].first >= lower; i++)
    ;
  dest.resize(i);
  srand(static_cast<unsigned int>(time(NULL)));
  return dest[rand() % dest.size()].second;
}

Result simulation::runSimulation(Task task)
{
  Result result(task.m_targetCount, task.toString());
  for (size_t i = 0; i < task.m_times; i++)
  {
    State state(task);
    while (!state.m_done)
      solveState(state);
    result.m_results.push_back(state.m_targetDeck.hit.size());
  }
  return result;
}

void simulation::solveState(State& state)
{
  if(state.m_targetDeck.left.empty())
  {
    size_t done(0);
    for(auto&& d : state.m_drones)
    {
      if(d.loc == state.m_homeLocation) done++;
    }
    if (done == state.m_drones.size()) 
    {
      state.m_done = true;
      return;
    }
  }
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
    if (!drone.optTarget)
      drone.optTarget = getNewTarget(state, drone);
    else if (!state.m_map[drone.optTarget.get().x][drone.optTarget.get().y])
    {
      drone.optTarget = getNewTarget(state, drone);
    }

    auto t = drone.optTarget.get();
    if (drone.loc.x != t.x)
      drone.loc.x += drone.loc.x > t.x ? -1 : 1;
    else if (drone.loc.y != t.y)
      drone.loc.y += drone.loc.y > t.y ? -1 : 1;
    drone.batteryLifeLeft--;

    if (drone.loc == drone.optTarget.get())
    {
      auto found = std::find(state.m_targetDeck.left.begin(),
                             state.m_targetDeck.left.end(),
                             drone.loc);
      if (drone.loc == state.m_homeLocation && found == state.m_targetDeck.left.end()) continue;
      state.m_targetDeck.hit.push_back(drone.loc);
      state.m_targetDeck.left.erase(found);
      state.m_map[drone.loc.x][drone.loc.y]--;
    }
  }

  if (done == state.m_drones.size()) state.m_done = true;
}
