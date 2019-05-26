#include "Parallel.hpp"
#include "Simulation.hpp"
#include "SimulationImpl.hpp"

#include <iostream>
#include <mutex>

#include <boost/asio.hpp>

RunConfig::RunConfig(Configuration config) : m_config(config)
{
}

RunConfig::~RunConfig()
{
}

void RunConfig::startTasks(const boost::filesystem::path& p)
{
  boost::asio::thread_pool pool;

  std::vector<Coordinate> homeLocations(m_config.getHomeLocations());
  std::vector<std::vector<size_t>> drones(m_config.getDrones());
  std::vector<size_t> targetCounts(m_config.getTargetCount());
  std::vector<std::vector<Coordinate>> targets(m_config.getTargets());
  std::vector<std::vector<ValueFunctionPercentage>> valueFunctionPercentage(
    m_config.getValueFunction());
  std::vector<Coordinate> sizes(m_config.getSizes());
  std::vector<double> diffPercentages(m_config.getDiffPercentage());

  std::ofstream fout;
  if (p.size() != 0)
  {
    fout.open(p.string());
    fout << "Results,,,,,,,,,\n";
    fout << "Average, Targets Hit, Size, Home Location, Drones, Target Count, "
            "Targets, "
            "Value Function, Diff Percentage, Times\n";
  }

  for (auto&& home : homeLocations)
  {
    for (auto&& droneDeck : drones)
    {
      if (targets.empty())
      {
        for (auto&& targetCount : targetCounts)
        {
          helpRun(fout, pool, home, droneDeck, targetCount, {});
        }
      }
      else
      {
        for (auto&& targetDeck : targets)
        {
          helpRun(fout, pool, home, droneDeck, 0, targetDeck);
        }
      }
    }
  }
  pool.join();
}

void RunConfig::helpRun(std::ofstream& fout,
                        boost::asio::thread_pool& pool,
                        const Coordinate& home,
                        const std::vector<size_t>& droneDeck,
                        const size_t& targetCount,
                        const std::vector<Coordinate>& targetDeck)
{
  static std::atomic<size_t> id;
  std::vector<std::vector<ValueFunctionPercentage>> valueFunctionPercentage(
    m_config.getValueFunction());
  std::vector<Coordinate> sizes(m_config.getSizes());
  std::vector<double> diffPercentages(m_config.getDiffPercentage());
  size_t times(m_config.getTimes());
  for (auto&& valueFunction : valueFunctionPercentage)
  {
    for (auto&& size : sizes)
    {
      for (auto&& diff : diffPercentages)
      {
        Task task(home,
                  droneDeck,
                  targetCount,
                  targetDeck,
                  valueFunction,
                  size,
                  diff,
                  times);
        boost::asio::post(pool, [task, &fout]() {
          static std::atomic<size_t> count;
          Result result = simulation::runSimulation(task);
          std::cout << count++ << std::endl;
          static std::mutex m;
          if (fout.is_open())
          {
            size_t targetsHit(0);
            for (auto&& j : result.m_results)
              targetsHit += j;
            double avg = (double)targetsHit / (double)result.m_results.size();
            std::lock_guard<std::mutex> lock(m);
            fout << avg / result.m_targetCount << "," << avg << ","
                 << result.m_taskString << std::flush << std::endl;
          }
        });
      }
    }
  }
}
