#include "State.hpp"

#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "StateDeserialization.hpp"
#include "StateSerialization.hpp"

namespace
{
  std::string jsonToString(const rapidjson::Document& doc)
  {
    rapidjson::StringBuffer buffer;

    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return std::string(buffer.GetString());
  }
}

State::State(const boost::filesystem::path& p)
  : m_drones(),
    m_targetDeck(),
    m_diffPercentage(),
    m_done(),
    m_homeLocation(),
    m_size(),
    m_map()
{
  if (!boost::filesystem::exists(p))
    throw std::runtime_error("file: " + p.string() + " does not exist");
  std::ifstream t(p.string());
  std::string str(
    (std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
  rapidjson::Document d;
  d.Parse(str.c_str());

  auto obj = d.GetObject();
  parseConfig(obj);
}

State::State(const Task& task)
  : m_drones(),
    m_targetDeck(),
    m_diffPercentage(task.m_diffPercentage),
    m_done(false),
    m_homeLocation(task.m_homeLocation),
    m_size(task.m_size),
    m_map()
{
  m_targetDeck.left = task.m_targets;
  m_map = new size_t*[m_size.x];
  for (size_t i = 0; i < m_size.x; ++i)
  {
    m_map[i] = new size_t[m_size.y];
    for(size_t j = 0; j < m_size.y; j++)
      m_map[i][j] = 0;
  }
  
  for (auto it = m_targetDeck.left.begin(); it != m_targetDeck.left.end();)
  {
    if (it->x >= m_size.x || it->x >= m_size.x)
    {
      it = m_targetDeck.left.erase(it);
      std::cout << "Not using Target that is off of the map" << std::endl;
    }
    else
      ++it;
  }

  for (auto&& c : m_targetDeck.left)
    m_map[c.x][c.y]++;
  auto itr = task.m_valueFunction.begin();
  size_t cur = 0;
  for (size_t i = 0; i < task.m_drones.size(); i++)
  {
    m_drones.push_back(Drone(task.m_drones[i],
                        task.m_homeLocation,
                        itr->valueFunction));
    if (cur / task.m_drones.size() > itr->percentage)
    {
      itr++;
      cur = 0;
    }
    else
      cur++;
  }

}

void State::parseConfig(const rapidjson::Value& obj)
{
  m_drones = state_deserialization::parseDrones(obj);
  m_targetDeck = state_deserialization::parseTargetDeck(obj);
  m_diffPercentage = state_deserialization::parseDiffPercentage(obj);
  m_done = state_deserialization::parseDone(obj);
  m_homeLocation = state_deserialization::parseHomeLocation(obj);
  m_size = state_deserialization::parseSize(obj);
  m_map = state_deserialization::parseMap(obj, m_size);
}

void State::writeOut(const boost::filesystem::path& p)
{
  rapidjson::Document d;
  d.SetObject();
  state_serialization::serializeDrones(d, m_drones);
  state_serialization::serializeTargetDeck(d, m_targetDeck);
  state_serialization::serializeDiffPercentage(d, m_diffPercentage);
  state_serialization::serializeDone(d, m_done);
  state_serialization::serializeHomeLocation(d, m_homeLocation);
  state_serialization::serializeSize(d, m_size);
  state_serialization::serializeMap(d, m_map, m_size);
  std::string msg(jsonToString(d));
  std::ofstream fout(p.string());
  fout << msg << std::endl;
}
