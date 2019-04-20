#ifndef STATE_HPP
#define STATE_HPP

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <rapidjson/document.h>

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
        ValueFunction vf,
        boost::optional<Coordinate> optT = boost::none)
    : batteryLifeLeft(battery),
      loc(c),
      valueFunctionEnum(vf),
      valueFunction(enumToFunction[vf]),
      optTarget(optT)
  {
  }
  size_t batteryLifeLeft;
  Coordinate loc;
  ValueFunction valueFunctionEnum;
  std::function<double(double)> valueFunction;
  boost::optional<Coordinate> optTarget;
};

struct State
{
  State(const boost::filesystem::path& p);
  State(const Task& task);

  void parseConfig(const rapidjson::Value& obj);
  void writeOut(const boost::filesystem::path& p);

  std::vector<Drone> m_drones;
  TargetDeck m_targetDeck;
  double m_diffPercentage;
  bool m_done;
  Coordinate m_homeLocation;
  Coordinate m_size;
  size_t** m_map;
};

#endif // !STATE_HPP
