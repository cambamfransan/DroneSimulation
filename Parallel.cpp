#include "Parallel.hpp"
#include "Simulation.hpp"
#include "SimulationImpl.hpp"

#include <iostream>

RunConfig::RunConfig(Configuration config)
  : m_ioc(), m_optCork(m_ioc), m_pThreads(), m_results(), m_resultsSize(), m_config(config)
{
}

RunConfig::~RunConfig()
{
  delete[] m_results;
}

void RunConfig::startTasks()
{
  for (uint64_t i = 0; i < std::thread::hardware_concurrency() - 1; i++)
    m_pThreads.push_back(
      std::make_shared<std::thread>([& m_ioc = m_ioc]() { m_ioc.run(); }));

  std::vector<Coordinate> homeLocations(m_config.getHomeLocations());
  std::vector<std::vector<size_t>> drones(m_config.getDrones());
  std::vector<size_t> targetCounts(m_config.getTargetCount());
  std::vector<std::vector<Coordinate>> targets(m_config.getTargets());
  std::vector<std::vector<ValueFunctionPercentage>> valueFunctionPercentage(
    m_config.getValueFunction());
  std::vector<Coordinate> sizes(m_config.getSizes());
  std::vector<double> diffPercentages(m_config.getDiffPercentage());
  size_t totalTargets =
    !targetCounts.empty() ? targetCounts.size() : targets.size();
  m_resultsSize = homeLocations.size() * drones.size() * totalTargets *
                  valueFunctionPercentage.size() * sizes.size() *
                  diffPercentages.size();
  m_results = new Result[m_resultsSize];

  for (auto&& home : homeLocations)
  {
    for (auto&& droneDeck : drones)
    {
      if (targets.empty())
      {
        for (auto&& targetCount : targetCounts)
        {
          helpRun(home, droneDeck, targetCount, {});
        }
      }
      else
      {
        for (auto&& targetDeck : targets)
        {
          helpRun(home, droneDeck, 0, targetDeck);
        }
      }
    }
  }
  m_optCork == boost::none;
  m_ioc.stop();
  m_ioc.run();
  for (uint64_t i = 0; i < std::thread::hardware_concurrency() - 1; i++)
    m_pThreads[i]->join();
}

void RunConfig::helpRun(const Coordinate& home,
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
        Task task(id++,
                  home,
                  droneDeck,
                  targetCount,
                  targetDeck,
                  valueFunction,
                  size,
                  diff,
                  times);
        m_ioc.post([ task, m_results = m_results ]() {
          Result result = simulation::runSimulation(task);
          m_results[result.m_id] = result;
        });
      }
    }
  }
}

void RunConfig::outputTasks(const boost::filesystem::path& p)
{
  if (!boost::filesystem::exists(p.parent_path()))
    throw std::runtime_error(
      "please give a path where the directory already exists.");

  std::ofstream fout(p.string());

  fout << "Results,,,,,,,,,\n";
  fout << "Targets Hit, Size, Home Location, Drones, Target Count, Targets, "
          "Value Function, Diff Percentage, Times\n";
  for (size_t i = 0; i < m_resultsSize; i++)
  {
    size_t targetsHit(0);
    for (auto&& j : m_results[i].m_results)
      targetsHit += j;
    double avg = (double)targetsHit / (double)m_results[i].m_results.size();
    fout << avg << "," << m_results[i].m_taskString << std::endl;
  }
}
