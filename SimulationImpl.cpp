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
    ss << val.x << " " << val.y << ",";
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
    m_targets(targets),
    m_targetCount(targetCount),
    m_valueFunction(valueFunction),
    m_size(size),
    m_diffPercentage(diffPercentage),
    m_times(times)
{
  if (targets.empty())
  {
    srand(time(NULL));
    for (size_t i = 0; i < targetCount; i++)
      targets.push_back(Coordinate(rand() % m_size.x, rand() % m_size.y));
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

Result::Result() : m_id(0), m_taskString(""), m_results() {}

Result simulation::runSimulation(Task task)
{
  // TODO calculate simulation
  Result toReturn(task.m_id, task.toString());
  for (size_t i = 0; i < task.m_times; i++)
    toReturn.m_results.push_back(i);
  return toReturn;
}