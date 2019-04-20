#ifndef STATE_DESERIALIZATION_HPP
#define STATE_DESERIALIZATION_HPP

#include "Coordinate.hpp"
#include "State.hpp"

namespace state_deserialization
{
  std::vector<Drone> parseDrones(const rapidjson::Value& obj);
  TargetDeck parseTargetDeck(const rapidjson::Value& obj);
  double parseDiffPercentage(const rapidjson::Value& obj);
  bool parseDone(const rapidjson::Value& obj);
  Coordinate parseHomeLocation(const rapidjson::Value& obj);
  Coordinate parseSize(const rapidjson::Value& obj);
  size_t** parseMap(const rapidjson::Value& obj, Coordinate size);
}

#endif // !STATE_DESERIALIZATION_HPP
