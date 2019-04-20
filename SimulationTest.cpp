#include "SimulationImpl.hpp"

#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(SimulationTest)

BOOST_AUTO_TEST_CASE(GetDroneTarget)
{
  std::vector<Coordinate> targets;
  targets.emplace_back(5, 4);
  targets.emplace_back(5, 3);
  targets.emplace_back(5, 2);
  targets.emplace_back(5, 1);
  targets.emplace_back(5, 0);
  std::vector<size_t> drones;
  drones.emplace_back(10);
  std::vector<ValueFunctionPercentage> valueFunctions;
  valueFunctions.push_back({ValueFunction::nn, 1});
  Task task(0,
            Coordinate(5, 5),
            drones,
            5,
            targets,
            valueFunctions,
            Coordinate(6, 6),
            0,
            1);
  State state(task);
  auto result = simulation::getNewTarget(state, state.m_drones[0]);
  BOOST_CHECK(result == Coordinate(5, 0));
  task.m_targets.emplace_back(0, 0);
  task.m_targetCount++;
  state = State(task);
  result = simulation::getNewTarget(state, state.m_drones[0]);
  BOOST_CHECK(result == Coordinate(5, 0));
  task.m_drones[0] = 20;
  state = State(task);
  result = simulation::getNewTarget(state, state.m_drones[0]);
  BOOST_CHECK(result == Coordinate(0, 0));
  task.m_valueFunction[0] = {ValueFunction::constant, 1};
  state = State(task);
  result = simulation::getNewTarget(state, state.m_drones[0]);
  BOOST_CHECK(result == Coordinate(5, 4));
}

BOOST_AUTO_TEST_CASE(SolveStateTest)
{
  std::vector<Coordinate> targets;
  targets.emplace_back(5, 1);
  targets.emplace_back(0, 0);
  targets.emplace_back(0, 5);
  std::vector<size_t> drones;
  drones.emplace_back(10);
  std::vector<ValueFunctionPercentage> valueFunctions;
  valueFunctions.push_back({ValueFunction::constant, 1});
  Task task(0,
            Coordinate(5, 5),
            drones,
            5,
            targets,
            valueFunctions,
            Coordinate(6, 6),
            0,
            1);
  State state(task);
  simulation::solveState(state);
  BOOST_CHECK(state.m_drones[0].loc.x == 5);
  BOOST_CHECK(state.m_drones[0].loc.y == 4);
  BOOST_CHECK(state.m_drones[0].optTarget.get() == Coordinate(5, 1));
  BOOST_CHECK(!state.m_done);
}

BOOST_AUTO_TEST_CASE(runSimulationTest)
{
  {
    std::vector<Coordinate> targets;
    targets.emplace_back(5, 1);
    targets.emplace_back(0, 0);
    targets.emplace_back(0, 5);
    std::vector<size_t> drones;
    drones.emplace_back(100);
    std::vector<ValueFunctionPercentage> valueFunctions;
    valueFunctions.push_back({ValueFunction::n, 1});
    Task task(0,
              Coordinate(5, 5),
              drones,
              5,
              targets,
              valueFunctions,
              Coordinate(6, 6),
              0,
              3);
    auto result = simulation::runSimulation(task);
    BOOST_CHECK(result.m_id == 0);
    BOOST_CHECK(result.m_results.size() == 3);
    for (size_t i = 0; i < result.m_results.size(); i++)
      BOOST_CHECK(result.m_results[i] == 3);

    BOOST_CHECK(result.m_taskString == "6 6,5 5,100 ,3,(5 1) (0 0) (0 5) ,n 1,0,3.");
  }
  {
    std::vector<Coordinate> targets;
    targets.emplace_back(5, 1);
    targets.emplace_back(0, 0);
    targets.emplace_back(0, 5);
    std::vector<size_t> drones;
    drones.emplace_back(100);
    std::vector<ValueFunctionPercentage> valueFunctions;
    valueFunctions.push_back({ValueFunction::n, 1});
    Task task(0,
              Coordinate(5, 5),
              drones,
              5,
              targets,
              valueFunctions,
              Coordinate(6, 6),
              0,
              3);
    auto result = simulation::runSimulation(task);
    BOOST_CHECK(result.m_id == 0);
    BOOST_CHECK(result.m_results.size() == 3);
    for (size_t i = 0; i < result.m_results.size(); i++)
      BOOST_CHECK(result.m_results[i] == 3);

    BOOST_CHECK(result.m_taskString == "6 6,5 5,100 ,3,(5 1) (0 0) (0 5) ,n 1,0,3.");
  }
}

BOOST_AUTO_TEST_SUITE_END()