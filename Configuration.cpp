#include <fstream>
#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>
#include <rapidjson/rapidjson.h>

#include "Configuration.hpp"

namespace
{

  Coordinate getCoordinateFromObject(const rapidjson::Value& rapidjsonObject)
  {
    auto obj = rapidjsonObject.GetObject();
    auto x = obj.FindMember("x");
    auto y = obj.FindMember("y");
    if (x == obj.MemberEnd())
      throw std::runtime_error("Could not find x part of coordinate");
    if (y == obj.MemberEnd())
      throw std::runtime_error("Could not find y part of coordinate");
    if (!x->value.IsUint64() || !y->value.IsUint64())
      throw std::runtime_error("x and y values must be unsigned integers");
    return {x->value.GetUint64(), y->value.GetUint64()};
  }

  Coordinate getCoordinateFromArray(const rapidjson::Value& rapidjsonObject)
  {
    auto obj = rapidjsonObject.GetArray();
    if (obj.Size() != 2)
      throw std::runtime_error(
        "If Coordinate is an array, there must be two and only two values");
    if (!obj[0].IsUint64() || !obj[1].IsUint64())
      throw std::runtime_error("Coordinate values must be integers");
    return {obj[0].GetUint64(), obj[1].GetUint64()};
  }

  std::vector<Coordinate> getCoordinates(
    const rapidjson::Value& rapidjsonObject, const std::string& key)
  {
    std::vector<Coordinate> toReturn;
    auto value = rapidjsonObject.FindMember(key.c_str());
    if (value == rapidjsonObject.MemberEnd())
      throw std::runtime_error("Could not find key: " + key);
    if (value->value.IsObject()) return {getCoordinateFromObject(value->value)};
    if (!value->value.IsArray())
      throw std::runtime_error(key + " must be either an object or array");

    auto values = value->value.GetArray();
    for (auto& val : values)
    {
      if (val.IsArray())
        toReturn.push_back(getCoordinateFromArray(val));
      else if (val.IsObject())
        toReturn.push_back(getCoordinateFromObject(val));
      else if (val.IsNumber() && values.Size() == 2)
      {
        toReturn.push_back(getCoordinateFromArray(value->value));
        break;
      }
      else
        throw std::runtime_error("Coordinate must be either object or array");
    }
    return toReturn;
  }

  std::vector<size_t> getUintVector(const rapidjson::Value& rapidjsonObject)
  {
    if (!rapidjsonObject.IsArray()) return {};
    auto rapidjsonVector = rapidjsonObject.GetArray();
    std::vector<size_t> toReturn;
    for (auto& val : rapidjsonVector)
    {
      if (!val.IsUint64())
        throw std::runtime_error(
          "Drones and TargetCount must be positive integers");
      toReturn.push_back(val.GetUint64());
    }
    return toReturn;
  }

  ValueFunctionPercentage getValueFunctionFromArray(
    const rapidjson::Value& value)
  {
    auto obj = value.GetArray();
    if (obj.Size() != 2)
      throw std::runtime_error(
        "ValueFuntion must have two arguements, function and percentage");
    if (!obj[0].IsString())
      throw std::runtime_error("function for ValueFunction must be a string");
    if (!obj[1].IsDouble())
      throw std::runtime_error("percentage for ValueFunction must be a double");

    return {stringToFunction[obj[0].GetString()], obj[1].GetDouble()};
  }

  std::vector<ValueFunctionPercentage> getValueFunctionArray(
    const rapidjson::Value& value)
  {
    std::vector<ValueFunctionPercentage> toReturn;
    auto deck = value.GetArray();
    for (auto& val : deck)
    {
      if (val.IsArray()) toReturn.push_back(getValueFunctionFromArray(val));
    }
    return toReturn;
  }
} // namespace

std::map<ValueFunction, std::function<double(double)>> enumToFunction = {
  {ValueFunction::n, [](const double& d) { return d; }},
  {ValueFunction::constant, [](const double& d) { return 1; }},
  {ValueFunction::logn, [](const double& d) { return log2(d); }},
  {ValueFunction::nlogn, [](const double& d) { return d * log2(d); }},
  {ValueFunction::n2, [](const double& d) { return 2 * d; }},
  {ValueFunction::n3, [](const double& d) { return 3 * d; }},
  {ValueFunction::nn, [](const double& d) { return d * d; }},
  {ValueFunction::nnn, [](const double& d) { return d * d * d; }}};

std::map<std::string, ValueFunction> stringToFunction = {
  {"n", ValueFunction::n},
  {"1", ValueFunction::constant},
  {"logn", ValueFunction::logn},
  {"nlogn", ValueFunction::nlogn},
  {"2n", ValueFunction::n2},
  {"3n", ValueFunction::n3},
  {"n^2", ValueFunction::nn},
  {"n^3", ValueFunction::nnn}};

std::map<ValueFunction, std::string> functionToString = {
  {ValueFunction::n, "n"},
  {ValueFunction::constant, "1"},
  {ValueFunction::logn, "logn"},
  {ValueFunction::nlogn, "nlogn"},
  {ValueFunction::n2, "2n"},
  {ValueFunction::n3, "3n"},
  {ValueFunction::nn, "n^2"},
  {ValueFunction::nnn, "n^3"}};

Configuration::Configuration(const boost::filesystem::path& p)
{
  std::ifstream t(p.string());
  std::string str(
    (std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

  try
  {
    rapidjson::Document d;
    d.Parse(str.c_str());

    auto obj = d.GetObject();
    parseConfig(obj);
  }
  catch (const std::exception& ex)
  {
    std::cout << "Could not get value, " << ex.what() << std::endl;
    exit(1); // TODO I don't like this, but it is quick and dirty
  }
}

void Configuration::parseConfig(const rapidjson::Value& obj)
{
  m_sizes = configuration::parseSize(obj);
  m_homeLocations = configuration::parseHomeCoordinates(obj);
  m_drones = configuration::parseDrones(obj);
  m_targetCount = configuration::parseTargetCount(obj);
  m_targets = configuration::parseTargets(obj);
  if (m_targetCount.empty() && m_targets.empty())
    throw std::runtime_error("Either Targets or TargetCount must be set");
  if (!m_targetCount.empty() && !m_targets.empty())
    throw std::runtime_error("Only one of Targets or TargetCount can be set");
  m_valueFunction = configuration::parseValueFunctions(obj);
  m_diffPercentage = configuration::parseDiff(obj);
  m_times = obj.FindMember("Times")->value.GetUint64();
}

std::vector<Coordinate> configuration::parseHomeCoordinates(
  const rapidjson::Value& value)
{
  return getCoordinates(value, "HomeLocation");
}

std::vector<Coordinate> configuration::parseSize(const rapidjson::Value& value)
{
  auto c = getCoordinates(value, "Size");
  std::cout << "" << std::endl;
  return c;
}

std::vector<std::vector<size_t>> configuration::parseDrones(
  const rapidjson::Value& value)
{
  auto dronesObj = value.FindMember("Drones");
  if (dronesObj == value.MemberEnd())
    throw std::runtime_error("Could not find key Drones");

  if (!dronesObj->value.IsArray())
    throw std::runtime_error("Drones needs to be an array");
  auto allDrones = dronesObj->value.GetArray();
  if (allDrones.Begin()->IsNumber()) return {getUintVector(dronesObj->value)};
  if (allDrones.Begin()->IsArray())
  {
    std::vector<std::vector<size_t>> toReturn;
    for (auto&& drones : allDrones)
    {
      toReturn.push_back(getUintVector(drones));
    }
    return toReturn;
  }

  throw std::runtime_error("Drones array must have 1 or more values");
  return {};
}

std::vector<size_t> configuration::parseTargetCount(
  const rapidjson::Value& value)
{
  auto obj = value.FindMember("TargetCount");
  if (obj == value.MemberEnd()) return {};
  if (obj->value.IsNumber()) return {obj->value.GetUint64()};
  if (!obj->value.IsArray())
    throw std::runtime_error("Target Count must be an integer or array");
  return getUintVector(obj->value);
}

std::vector<std::vector<Coordinate>> configuration::parseTargets(
  const rapidjson::Value& value)
{
  auto obj = value.FindMember("Targets");
  if (obj == value.MemberEnd()) return {};
  if (!obj->value.IsArray())
    throw std::runtime_error("Targets must be an array");

  auto targetsArray = obj->value.GetArray();
  if (targetsArray.Begin()->IsObject() ||
      (targetsArray.Begin()->IsArray() &&
       targetsArray.Begin()->GetArray().Begin()->IsNumber()))
  {
    std::vector<Coordinate> toReturn;
    for (auto& target : targetsArray)
    {
      if (target.IsObject())
        toReturn.push_back(getCoordinateFromObject(target));
      else if (target.IsArray())
        toReturn.push_back(getCoordinateFromArray(target));
      else
        throw std::runtime_error("Coordinate must be either object or array");
    }
    return {toReturn};
  }
  std::vector<std::vector<Coordinate>> toReturn;
  for (auto&& targets : targetsArray)
  {
    auto targetDeck = targets.GetArray();
    std::vector<Coordinate> toAdd;
    for (auto& target : targetDeck)
    {
      if (target.IsObject())
        toAdd.push_back(getCoordinateFromObject(target));
      else if (target.IsArray())
        toAdd.push_back(getCoordinateFromArray(target));
      else
        throw std::runtime_error("Coordinate must be either object or array");
    }
    toReturn.push_back(toAdd);
  }
  return toReturn;
}

std::vector<std::vector<ValueFunctionPercentage>> configuration::
  parseValueFunctions(const rapidjson::Value& value)
{
  auto obj = value.FindMember("ValueFunction");
  if (obj == value.MemberEnd())
    throw std::runtime_error("ValueFunction must be set");
  if (obj->value.IsString())
    return {{{stringToFunction[obj->value.GetString()], 1}}};
  // Single or double array
  if (!obj->value.IsArray())
    throw std::runtime_error("ValueFunction must be a string or array");
  auto arr = obj->value.GetArray();
  std::vector<std::vector<ValueFunctionPercentage>> toReturn;
  if (arr.Begin()->IsString())
  {
    for (auto& func : arr)
      toReturn.push_back({{stringToFunction[func.GetString()], 1}});
    return toReturn;
  }
  if (!arr.Begin()->IsArray())
    throw std::runtime_error(
      "ValueFunction is badly constructed, please read the documentation");
  if (!arr.Begin()->GetArray().Begin()->IsArray())
    return {getValueFunctionArray(obj->value)};

  for (auto& inner : arr)
  {
    if (!inner.IsArray())
      throw std::runtime_error(
        "ValueFunction is badly constructed, please read the documentation");
    toReturn.push_back(getValueFunctionArray(inner));
  }
  return toReturn;
}

std::vector<double> configuration::parseDiff(const rapidjson::Value& value)
{
  auto obj = value.FindMember("DiffPercentage");
  if (obj == value.MemberEnd())
    throw std::runtime_error("Could not find key DiffPercentage");

  if (obj->value.IsDouble()) return {obj->value.GetDouble()};

  if (!obj->value.IsArray())
    throw std::runtime_error("DiffPercentage must be a double or array");

  auto deck = obj->value.GetArray();
  std::vector<double> toReturn;
  for (auto& diff : deck)
  {
    if (!diff.IsDouble())
      throw std::runtime_error("DiffPercentages must be doubles");
    toReturn.push_back(diff.GetDouble());
  }
  return toReturn;
}

// Coordinate m_homeLocation;
// std::vector<size_t> m_drones;
// size_t m_targetCount;
// ValueFunction m_valueFunction;
// Coordinate m_size;
// double diffPercentage;
// size_t times;
