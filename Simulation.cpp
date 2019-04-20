#include "Simulation.hpp"

#include <atomic>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string/join.hpp>

#include "Configuration.hpp"

void vectValFuncToString(std::stringstream& ss,
                         std::vector<ValueFunctionPercentage>& vec)
{
  for (auto&& val : vec)
    ss << functionToString[val.valueFunction] << " " << val.percentage;
  ss << ",";
}

void vectSizeTToString(std::stringstream& ss, std::vector<size_t>& vec)
{
  for (auto&& val : vec)
    ss << val << " ";
  ss << ",";
}

void vectCoordToString(std::stringstream& ss, std::vector<Coordinate>& vec)
{
  for (auto&& val : vec)
    ss << "(" <<val.x << " " << val.y << ") ";
  ss << ",";
}

Task::Task(size_t id,
           Coordinate homeLocation,
           std::vector<size_t> drones,
           size_t targetCount,
           std::vector<Coordinate> targets,
           std::vector<ValueFunctionPercentage> valueFunction,
           Coordinate size,
           double diffPercentage,
           size_t times)
  : m_id(id),
    m_homeLocation(homeLocation),
    m_drones(drones),
    m_targetCount(targetCount),
    m_targets(targets),
    m_valueFunction(valueFunction),
    m_size(size),
    m_diffPercentage(diffPercentage),
    m_times(times)
{
  if (m_homeLocation.x >= m_size.x || m_homeLocation.y >= m_size.y)
  {
    std::cout << "Cannot set home location off of the board, setting it to "
                 "closest point"
              << std::endl;
    m_homeLocation.x = m_size.x - 1;
    m_homeLocation.y = m_size.y - 1;
  }
  if (targets.empty())
  {
    srand(time(NULL));
    for (size_t i = 0; i < targetCount; i++)
      m_targets.push_back(Coordinate(rand() % (m_size.x - 1), rand() % (m_size.y - 1)));
  }
  else
    m_targetCount = targets.size();
}

std::string Task::toString()
{
  std::stringstream ss;

  ss << m_size.x << " " << m_size.y << ",";
  ss << m_homeLocation.x << " " << m_homeLocation.y << ",";
  vectSizeTToString(ss, m_drones);
  ss << m_targetCount << ",";
  vectCoordToString(ss, m_targets);
  vectValFuncToString(ss, m_valueFunction);
  ss << m_diffPercentage << ",";
  ss << m_times << ".";

  return ss.str();
}

Result::Result(size_t id, std::string result)
  : m_id(id), m_taskString(result), m_results()
{
}

Result::Result() : m_id(0), m_taskString(""), m_results()
{
}
