#ifndef STATE_SERIALIZATION_HPP
#define STATE_SERIALIZATION_HPP

#include "Coordinate.hpp"
#include "State.hpp"

namespace state_serialization
{
  void serializeDrones(rapidjson::Document& d,
                       const std::vector<Drone>& drones);
  void serializeTargetDeck(rapidjson::Document& d,
                           const TargetDeck& targetDeck);
  void serializeDiffPercentage(rapidjson::Document& d,
                               const double& diffPercentage);
  void serializeDone(rapidjson::Document& d, const bool done);
  void serializeHomeLocation(rapidjson::Document& d,
                             const Coordinate& homeLocation);
  void serializeSize(rapidjson::Document& d, const Coordinate& size);
  void serializeMap(rapidjson::Document& d,
                    size_t** map,
                    const Coordinate& size);
}

#endif // !STATE_SERIALIZATION_HPP
