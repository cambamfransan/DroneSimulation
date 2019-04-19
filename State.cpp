#include "State.hpp"

State::State(const boost::filesystem::path& p)
  : m_drones(),
    m_targetDeck(),
    m_diffPercentage(),
    m_done(),
    m_homeLocation(),
    m_size(),
    map()
{
  if (!boost::filesystem::exists(p))
    throw std::runtime_error("file: " + p.string() + " does not exist");

  // TODO
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
  m_map = new bool*[m_size.x];
  for (int i = 0; i < m_size.x; ++i)
    m_map[i] = new bool[m_size.y];
  for (auto&& c : m_targetDeck.left)
    m_map[c.x][c.y] = true;
  auto itr = task.m_valueFunction.begin();
  size_t cur = 0;
  for (size_t i = 0; i < task.m_drones.size(); i++)
  {
    m_drones.push_back(Drone(task.m_drones[i],
                        task.m_homeLocation,
                        enumToFunction[itr->valueFunction],
                        Coordinate()));
    if (cur / task.m_drones.size() > itr->percentage)
    {
      itr++;
      cur = 0;
    }
    else
      cur++;
  }

}
