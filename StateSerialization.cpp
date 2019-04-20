#include "StateSerialization.hpp"

#include <iostream>

namespace
{
  void addSizetToObject(rapidjson::Document& doc,
                        rapidjson::Value& obj,
                        std::string member,
                        size_t value);

  void addDoubleToDoc(rapidjson::Document& doc,
                      std::string member,
                      double value)
  {
    rapidjson::Value rapidValue;
    rapidValue.SetDouble(value);
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    doc.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addBoolToDoc(rapidjson::Document& doc,
                    const std::string member,
                    bool value)
  {
    rapidjson::Value rapidValue(value);
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    doc.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addCoordinateToDoc(rapidjson::Document& doc,
                          const std::string& member,
                          const Coordinate& value)
  {
    rapidjson::Value rapidValue(rapidjson::kObjectType);
    addSizetToObject(doc, rapidValue, "x", value.x);
    addSizetToObject(doc, rapidValue, "y", value.y);
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    doc.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addObjectToDoc(rapidjson::Document& doc,
                      const std::string member,
                      rapidjson::Value& value)
  {
    rapidjson::Value rapidValue(rapidjson::kObjectType);
    rapidValue = value;
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    doc.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addArrayToDoc(rapidjson::Document& doc,
                     const std::string member,
                     rapidjson::Value& value)
  {
    rapidjson::Value rapidValue(rapidjson::kArrayType);
    rapidValue = value;
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    doc.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addSizetToObject(rapidjson::Document& doc,
                        rapidjson::Value& obj,
                        std::string member,
                        size_t value)
  {
    rapidjson::Value rapidValue;
    rapidValue.SetUint64(value);
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    obj.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addObjectToObject(rapidjson::Document& doc,
                         rapidjson::Value& obj,
                         const std::string member,
                         rapidjson::Value& value)
  {
    rapidjson::Value rapidValue(rapidjson::kObjectType);
    rapidValue = value;
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    obj.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addCoordinateToObject(rapidjson::Document& doc,
                             rapidjson::Value& obj,
                             const std::string& member,
                             const Coordinate& value)
  {
    rapidjson::Value rapidValue(rapidjson::kObjectType);
    addSizetToObject(doc, rapidValue, "x", value.x);
    addSizetToObject(doc, rapidValue, "y", value.y);
    addObjectToObject(doc, obj, member, rapidValue);
  }

  void addStringToObject(rapidjson::Document& doc,
                         rapidjson::Value& obj,
                         const std::string member,
                         const std::string value)
  {
    rapidjson::Value rapidValue(
      value.c_str(), static_cast<int>(value.size()), doc.GetAllocator());
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    obj.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addArrayToObject(rapidjson::Document& doc,
                        rapidjson::Value& obj,
                        const std::string member,
                        rapidjson::Value& value)
  {
    rapidjson::Value rapidValue(rapidjson::kArrayType);
    rapidValue = value;
    rapidjson::Value index(
      member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
    obj.AddMember(index, rapidValue, doc.GetAllocator());
  }

  void addObjectToArray(rapidjson::Document& doc,
                        rapidjson::Value& arr,
                        rapidjson::Value& value)
  {
    rapidjson::Value rapidValue(rapidjson::kObjectType);
    rapidValue = value;
    arr.PushBack(rapidValue, doc.GetAllocator());
  }

  void addCoordinateToArray(rapidjson::Document& doc,
                            rapidjson::Value& arr,
                            const Coordinate& value)
  {
    rapidjson::Value rapidValue(rapidjson::kObjectType);
    addSizetToObject(doc, rapidValue, "x", value.x);
    addSizetToObject(doc, rapidValue, "y", value.y);
    arr.PushBack(rapidValue, doc.GetAllocator());
  }

  void addSizetToArray(rapidjson::Document& doc,
                       rapidjson::Value& arr,
                       const size_t value)
  {
    rapidjson::Value rapidValue(rapidjson::kNumberType);
    rapidValue.SetUint64(value);
    arr.PushBack(rapidValue, doc.GetAllocator());
  }

  void addArrayToArray(rapidjson::Document& doc,
                       rapidjson::Value& arr,
                       rapidjson::Value& value)
  {
    rapidjson::Value rapidValue(rapidjson::kArrayType);
    rapidValue = value;
    arr.PushBack(rapidValue, doc.GetAllocator());
  }
}

void state_serialization::serializeDrones(rapidjson::Document& d,
                                          const std::vector<Drone>& drones)
{
  rapidjson::Value dronesArray(rapidjson::kArrayType);
  for (auto&& drone : drones)
  {
    rapidjson::Value v(rapidjson::kObjectType);
    addSizetToObject(d, v, "batteryLifeLeft", drone.batteryLifeLeft);
    addCoordinateToObject(d, v, "Location", drone.loc);
    addStringToObject(
      d, v, "ValueFunction", functionToString[drone.valueFunctionEnum]);
    if (drone.optTarget)
      addCoordinateToObject(d, v, "Target", drone.optTarget.get());
    addObjectToArray(d, dronesArray, v);
  }
  addArrayToDoc(d, "Drones", dronesArray);
}
void state_serialization::serializeTargetDeck(rapidjson::Document& d,
                                              const TargetDeck& targetDeck)
{
  rapidjson::Value targetDeckObj(rapidjson::kObjectType);

  rapidjson::Value targetDeckHitArr(rapidjson::kArrayType);
  for (auto&& t : targetDeck.hit)
    addCoordinateToArray(d, targetDeckHitArr, t);
  addArrayToObject(d, targetDeckObj, "Hit", targetDeckHitArr);

  rapidjson::Value targetDeckLeftArr(rapidjson::kArrayType);
  for (auto&& t : targetDeck.left)
    addCoordinateToArray(d, targetDeckLeftArr, t);
  addArrayToObject(d, targetDeckObj, "Left", targetDeckLeftArr);

  addObjectToDoc(d, "TargetDeck", targetDeckObj);
}
void state_serialization::serializeDiffPercentage(rapidjson::Document& d,
                                                  const double& diffPercentage)
{
  addDoubleToDoc(d, "DiffPercentage", diffPercentage);
}
void state_serialization::serializeDone(rapidjson::Document& d, const bool done)
{
  addBoolToDoc(d, "Done", done);
}
void state_serialization::serializeHomeLocation(rapidjson::Document& d,
                                                const Coordinate& homeLocation)
{
  addCoordinateToDoc(d, "HomeLocation", homeLocation);
}
void state_serialization::serializeSize(rapidjson::Document& d,
                                        const Coordinate& size)
{
  addCoordinateToDoc(d, "Size", size);
}
void state_serialization::serializeMap(rapidjson::Document& d,
                                       size_t** map,
                                       const Coordinate& size)
{
  rapidjson::Value outer(rapidjson::kArrayType);
  for (int i = 0; i < size.x; i++)
  {
    rapidjson::Value inner(rapidjson::kArrayType);
    for (int j = 0; j < size.y; j++)
    {
      addSizetToArray(d, inner, map[i][j]);
    }
    addArrayToArray(d, outer, inner);
  }
  addArrayToDoc(d, "Map", outer);
}