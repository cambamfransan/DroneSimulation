#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <cmath>
#include <functional>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include "Coordinate.hpp"

enum class ValueFunction
{
  n,
  constant,
  logn,
  nlogn,
  n2,
  n3,
  nn,
  nnn
};

struct ValueFunctionPercentage
{
  ValueFunction valueFunction;
  double percentage;
};

class Configuration
{
public:
  Configuration(const boost::filesystem::path& p);

  void parseConfig(const rapidjson::Value& v);

  std::vector<size_t> getTargetCount() { return m_targetCount; }

  std::vector<std::vector<size_t>> getDrones() { return m_drones; }

private:
  std::vector<Coordinate> m_homeLocations;
  std::vector<std::vector<size_t>> m_drones;
  std::vector<size_t> m_targetCount;
  std::vector<std::vector<Coordinate>> m_targets;
  std::vector<std::vector<ValueFunctionPercentage>> m_valueFunction;
  std::vector<Coordinate> m_sizes;
  std::vector<double> m_diffPercentage;
  size_t m_times;
};

namespace configuration
{
  std::vector<Coordinate> parseHomeCoordinates(const rapidjson::Value& value);
  std::vector<Coordinate> parseSize(const rapidjson::Value& value);
  std::vector<std::vector<size_t>> parseDrones(const rapidjson::Value& value);
  std::vector<size_t> parseTargetCount(const rapidjson::Value& value);
  std::vector<std::vector<Coordinate>> parseTargets(
    const rapidjson::Value& value);
  std::vector<std::vector<ValueFunctionPercentage>> parseValueFunctions(
    const rapidjson::Value& value);
  std::vector<double> parseDiff(const rapidjson::Value& value);
} // namespace configuration

#endif // !CONFIGURATION_HPP
