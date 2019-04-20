#ifndef SIMULATION_HPP
#define SIMULATION_HPP

//#include "Parallel.hpp"
#include "Configuration.hpp"

struct Task
{
  Task(size_t id,
       Coordinate homeLocation,
       std::vector<size_t> drones,
       size_t targetCount,
       std::vector<Coordinate> targets,
       std::vector<ValueFunctionPercentage> valueFunction,
       Coordinate size,
       double diffPercentage,
       size_t times);
  size_t m_id;
  Coordinate m_homeLocation;
  std::vector<size_t> m_drones;
  size_t m_targetCount;
  std::vector<Coordinate> m_targets;
  std::vector<ValueFunctionPercentage> m_valueFunction;
  Coordinate m_size;
  double m_diffPercentage;
  size_t m_times;
  std::string toString();
};

struct Result
{
  Result();
  Result(size_t id, std::string result);
  size_t m_id;
  std::string m_taskString;
  std::vector<size_t> m_results;
};

#endif // !SIMULATION_HPP
