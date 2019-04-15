#ifndef STATE_HPP
#define STATE_HPP

#include "Configuration.hpp"

#include "Coordinate.hpp"

struct TargetDeck
{
  std::vector<Coordinate> left;
  std::vector<Coordinate> hit;
};

struct Drone
{
  Coordinate loc;
  bool done;
};

class State
{
public:
  State(const boost::filesystem::path& p);
  State(const Configuration& config);

private:
  std::vector<Drone> m_drones;
  TargetDeck m_targetDeck;
  Configuration m_config;
  bool m_done;
};

#endif // !STATE_HPP
