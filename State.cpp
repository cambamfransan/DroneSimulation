#include "State.hpp"

namespace
{
  //Coordinate getCoordinate(const Coordinate& size)
  //{
  //  // get random numbers
  //}

}

State::State(const boost::filesystem::path& p)
  : m_drones(), m_targetDeck(), m_config(p), m_done(false)
{
}

State::State(const Configuration& config)
  : m_drones(), m_targetDeck(), m_config(config), m_done(false)
{
  //// need to seed it
  //auto targetCount = m_config.getTargetCount();
  //for (size_t i = 0; i < targetCount; i++)
  //  m_targetDeck
}
