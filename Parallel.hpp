#ifndef PARALLEL_HPP
#define PARALLEL_HPP

#include <thread>
#include <vector>

#include <boost/asio/io_context.hpp>
#include <boost/optional.hpp>

#include "Configuration.hpp"
#include "Simulation.hpp"

class RunConfig
{
public:
  RunConfig(Configuration config);
  ~RunConfig();

  void startTasks();

  void outputTasks(const boost::filesystem::path& p);

private:
  void helpRun(const Coordinate& home,
               const std::vector<size_t>& droneDeck,
               const size_t& targetCount,
               const std::vector<Coordinate>& targetDeck);

  boost::asio::io_context m_ioc;
  boost::optional<boost::asio::io_context::work> m_optCork;
  std::vector<std::shared_ptr<std::thread>> m_pThreads;
  Result* m_results;
  size_t m_resultsSize;
  Configuration m_config;
};

#endif // !PARALLEL_HPP
