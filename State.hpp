#ifndef STATE_HPP
#define STATE_HPP

#include "Coordinate.hpp"
#include "Simulation.hpp"

struct TargetDeck
{
  std::vector<Coordinate> left;
  std::vector<Coordinate> hit;
};

struct Drone
{
  Drone(size_t battery,
        Coordinate c,
        std::function<double(double)> vf,
        Coordinate t)
    : batteryLifeLeft(battery), loc(c), valueFunction(vf), target(t)
  {
  }
  size_t batteryLifeLeft;
  Coordinate loc;
  std::function<double(double)> valueFunction;
  Coordinate target;
};

struct State
{
  State(const boost::filesystem::path& p);
  State(const Task& task);

  std::vector<Drone> m_drones;
  TargetDeck m_targetDeck;
  double m_diffPercentage;
  bool m_done;
  Coordinate m_homeLocation;
  Coordinate m_size;
  bool** m_map;
};

#endif // !STATE_HPP
