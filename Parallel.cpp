#include "Parallel.hpp"
#include "Simulation.hpp"

RunConfig::RunConfig(Configuration config)
  : m_ioc(), m_optCork(m_ioc), m_pThreads(), m_config(config)
{
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
  size_t times(m_config.getTimes());
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
        Task task(id++, home,
                  droneDeck,
                  targetCount,
                  targetDeck,
                  valueFunction,
                  size,
                  diff,
                  times);
        m_ioc.post([task, m_results = m_results]() {
          Result result = simulation::runSimulation(task);
          m_results[result.m_id] = result;
        });
      }
    }
  }
}

//  ss << m_size.x << " " << m_size.y << ",";
//ss << m_homeLocation.x << " " << m_homeLocation.y << ",";
//vectSizeTToString(ss, m_drones);
//ss << m_targetCount << ",";
//if (m_printTargets) vectCoordToString(ss, m_targets);
//vectValFuncToString(ss, m_valueFunction);
//ss << m_diffPercentage << ",";
//ss << m_times << ".";

void RunConfig::outputTasks(const boost::filesystem::path& p)
{
  if(!boost::filesystem::exists(p.parent_path()))
    throw std::runtime_error("please give a path where the directory already exists.");

  std::ofstream fout(p.string());

  fout << "Results,,,,,,,,,\n";
  fout << "Targets, Size, Home Location, Drones, Target Count, Targets, Value Function, Diff Percentage, Times\n";
  for (size_t i = 0; i < m_resultsSize; i++)
  {
    size_t targetsHit(0);
    for (auto&& j : m_results[i].m_results)
      targetsHit += j;
    fout <<( float)targetsHit / (float)m_results[i].m_results.size() << ",";
    fout << m_results[i].m_taskString << std::endl;
  }
}
