#include "StateDeserialization.hpp"

#include <iostream>

#include "Configuration.hpp"

namespace
{
  size_t getSizet(const rapidjson::Value& obj, const std::string& key)
  {
    auto keyObj = obj.FindMember(key.c_str());
    if (keyObj == obj.MemberEnd())
      throw std::runtime_error("Could not find key: " + key);
    if (!keyObj->value.IsUint())
      throw std::runtime_error(key + " must be a uint");
    return keyObj->value.GetUint();
  }

  boost::optional<Coordinate> getCoordinate(const rapidjson::Value& obj,
                                            const std::string& key,
                                            bool fail = true)
  {
    auto keyObj = obj.FindMember(key.c_str());
    if (keyObj == obj.MemberEnd() && fail)
      throw std::runtime_error("Could not find key: " + key);
    else if (keyObj == obj.MemberEnd())
      return boost::none;
    if (!keyObj->value.IsObject())
      throw std::runtime_error(key + " must be an object");
    return Coordinate(
      getSizet(keyObj->value, "x"), getSizet(keyObj->value, "y"));
  }

  ValueFunction getValueFunction(const rapidjson::Value& obj,
                                 const std::string& key)
  {
    auto keyObj = obj.FindMember(key.c_str());
    if (keyObj == obj.MemberEnd())
      throw std::runtime_error("Could not find key: " + key);
    if (!keyObj->value.IsString())
      throw std::runtime_error(key + " must be a uint");
    return stringToFunction[keyObj->value.GetString()];
  }

  std::vector<Coordinate> getCoordinateVector(const rapidjson::Value& obj)
  {
    std::vector<Coordinate> toReturn;
    auto objArr = obj.GetArray();
    for (auto&& o : objArr)
    {
      toReturn.emplace_back(Coordinate(getSizet(o, "x"), getSizet(o, "y")));
    }
    return toReturn;
  }
}
std::vector<Drone> state_deserialization::parseDrones(
  const rapidjson::Value& obj)
{
  std::vector<Drone> toReturn;
  auto dronesObj = obj.FindMember("Drones");
  if (dronesObj == obj.MemberEnd())
    throw std::runtime_error("Could not find Drones in State");
  if (!dronesObj->value.IsArray())
    throw std::runtime_error("Drones must be an array");
  auto drones = dronesObj->value.GetArray();
  for (auto&& drone : drones)
  {
    if (!drone.IsObject())
      throw std::runtime_error("Each drone must be an object");
    size_t battery = getSizet(drone, "batteryLifeLeft");
    Coordinate loc = getCoordinate(drone, "Location").get();
    ValueFunction vf = getValueFunction(drone, "ValueFunction");
    boost::optional<Coordinate> target = getCoordinate(drone, "Target", false);
    toReturn.emplace_back(battery, loc, vf, target);
  }
  return toReturn;
}

TargetDeck state_deserialization::parseTargetDeck(const rapidjson::Value& obj)
{
  TargetDeck toReturn;
  auto targetDeckObj = obj.FindMember("TargetDeck");
  if (targetDeckObj == obj.MemberEnd())
    throw std::runtime_error("Could not find TargetDeck in State");
  if (!targetDeckObj->value.IsObject())
    throw std::runtime_error("TargetDeck must be an object");
  auto targetDeck = targetDeckObj->value.GetObject();

  auto hitObj = targetDeck.FindMember("Hit");
  if (hitObj != targetDeck.MemberEnd())
  {
    if (!hitObj->value.IsArray())
      throw std::runtime_error("TargetDeck Hit must be an array");
    toReturn.hit = getCoordinateVector(hitObj->value);
  }
  auto leftObj = targetDeck.FindMember("Left");
  if (leftObj != targetDeck.MemberEnd())
  {
    if (!leftObj->value.IsArray())
      throw std::runtime_error("TargetDeck Left must be an array");
    toReturn.left = getCoordinateVector(leftObj->value);
  }
  return toReturn;
}
double state_deserialization::parseDiffPercentage(const rapidjson::Value& obj)
{
  auto diffPercentageObj = obj.FindMember("DiffPercentage");
  if (diffPercentageObj == obj.MemberEnd())
    throw std::runtime_error("Could not find DiffPercentage in State");

  if (!diffPercentageObj->value.IsNumber())
    throw std::runtime_error("DiffPercentage must be a number");

  return diffPercentageObj->value.GetDouble();
}
bool state_deserialization::parseDone(const rapidjson::Value& obj)
{
  auto doneObj = obj.FindMember("Done");
  if (doneObj == obj.MemberEnd())
    throw std::runtime_error("Could not find Done in State");

  if (!doneObj->value.IsBool()) throw std::runtime_error("Done must be a bool");

  return doneObj->value.GetBool();
}
Coordinate state_deserialization::parseHomeLocation(const rapidjson::Value& obj)
{
  return getCoordinate(obj, "HomeLocation").get();
}
Coordinate state_deserialization::parseSize(const rapidjson::Value& obj)
{
  return getCoordinate(obj, "Size").get();
}
size_t** state_deserialization::parseMap(const rapidjson::Value& obj,
                                         Coordinate size)
{
  auto mapObj = obj.FindMember("Map");
  if (mapObj == obj.MemberEnd())
    throw std::runtime_error("Could not find Map in State");

  if (!mapObj->value.IsArray())
    throw std::runtime_error("Map must be an array");

  size_t** toReturn;
  toReturn = new size_t*[size.x];
  for (size_t i = 0; i < size.x; ++i)
  {
    toReturn[i] = new size_t[size.y];
    for (size_t j = 0; j < size.y; j++)
      toReturn[i][j] = 0;
  }

  auto map = mapObj->value.GetArray();
  int i, j;
  i = 0;
  for (auto&& col : map)
  {
    if (!col.IsArray()) throw std::runtime_error("Map must be a 2d array");
    auto c = col.GetArray();
    j = 0;
    for (auto&& el : c)
    {
      if (!el.IsUint())
        throw std::runtime_error("Each element must be a unsigned int");
      toReturn[i][j] = el.GetUint();
      j++;
    }
    i++;
  }

  return toReturn;
}