#ifndef PARALLEL_HPP
#define PARALLEL_HPP

#include <thread>
#include <vector>

#include <boost/asio/thread_pool.hpp>
#include <boost/optional.hpp>

#include "Configuration.hpp"
#include "Simulation.hpp"

class RunConfig
{
public:
  RunConfig(Configuration config);
  ~RunConfig();

  void startTasks(const boost::filesystem::path& p = "");

private:
  void helpRun(std::ofstream& fout,
               boost::asio::thread_pool& pool,
               const Coordinate& home,
               const std::vector<size_t>& droneDeck,
               const size_t& targetCount,
               const std::vector<Coordinate>& targetDeck);

  Configuration m_config;
};

#endif // !PARALLEL_HPP
