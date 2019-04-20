#include <iostream>

#include <boost/test/unit_test.hpp>

#include "Configuration.hpp"
#include "Coordinate.hpp"
#include "SimulationImpl.hpp"
#include "State.hpp"

BOOST_AUTO_TEST_SUITE(SolveStateTestSuite)

BOOST_AUTO_TEST_CASE(SolveStateTest)
{
  std::vector<Coordinate> targets;
  targets.emplace_back(4, 1);
  targets.emplace_back(0, 0);
  targets.emplace_back(0, 4);
  std::vector<size_t> drones;
  drones.emplace_back(100);
  std::vector<ValueFunctionPercentage> valueFunctions;
  valueFunctions.push_back({ValueFunction::logn, 1});
  Task task(0,
            Coordinate(4, 4),
            drones,
            5,
            targets,
            valueFunctions,
            Coordinate(5, 5),
            0,
            3);
  State state(task);
  std::string file("test.csv");
  state.writeOut(file);
  std::ifstream t(file);
  std::string line;
  std::getline(t, line);
  t.close();
  std::string ans(
    "{\"Drones\":[{\"batteryLifeLeft\":100,\"Location\":{\"x\":4,"
    "\"y\":4},\"ValueFunction\":\"logn\"}],\"TargetDeck\":{\"Hit\":["
    "],\"Left\":[{\"x\":4,\"y\":1},{\"x\":0,\"y\":0},{\"x\":0,"
    "\"y\":4}]},\"DiffPercentage\":0.0,\"Done\":false,"
    "\"HomeLocation\":{\"x\":4,\"y\":4},\"Size\":{\"x\":5,\"y\":"
    "5},\"Map\":[[1,0,0,0,1],[0,0,0,0,0],[0,0,0,0,0],[0,"
    "0,0,0,0],[0,1,0,0,0]]}");
  BOOST_CHECK(line == ans);
  state = State(file);
  simulation::solveState(state);
  state.writeOut(file);
  t = std::ifstream(file);
  std::getline(t, line);
  t.close();
  ans = "{\"Drones\":[{\"batteryLifeLeft\":99,\"Location\":{\"x\":4,"
        "\"y\":3},\"ValueFunction\":\"logn\",\"Target\":{\"x\":4,\"y\":1}}],"
        "\"TargetDeck\":{\"Hit\":[],\"Left\":[{\"x\":4,\"y\":1},{\"x\":0,\"y\":"
        "0},{\"x\":0,\"y\":4}]},\"DiffPercentage\":0.0,\"Done\":false,"
        "\"HomeLocation\":{\"x\":4,\"y\":4},\"Size\":{\"x\":5,\"y\":"
        "5},\"Map\":[[1,0,0,0,1],[0,0,0,0,0],[0,0,0,0,0],[0,"
        "0,0,0,0],[0,1,0,0,0]]}";
  BOOST_CHECK(line == ans);
  state = State(file);
  simulation::solveState(state);
  state.writeOut(file);
  t = std::ifstream(file);
  std::getline(t, line);
  t.close();
  ans = "{\"Drones\":[{\"batteryLifeLeft\":98,\"Location\":{\"x\":4,"
        "\"y\":2},\"ValueFunction\":\"logn\",\"Target\":{\"x\":4,\"y\":1}}],"
        "\"TargetDeck\":{\"Hit\":[],\"Left\":[{\"x\":4,\"y\":1},{\"x\":0,\"y\":"
        "0},{\"x\":0,\"y\":4}]},\"DiffPercentage\":0.0,\"Done\":false,"
        "\"HomeLocation\":{\"x\":4,\"y\":4},\"Size\":{\"x\":5,\"y\":"
        "5},\"Map\":[[1,0,0,0,1],[0,0,0,0,0],[0,0,0,0,0],[0,"
        "0,0,0,0],[0,1,0,0,0]]}";
  BOOST_CHECK(line == ans);
  state = State(file);
  simulation::solveState(state);
  state.writeOut(file);
  t = std::ifstream(file);
  std::getline(t, line);
  t.close();
  ans = "{\"Drones\":[{\"batteryLifeLeft\":97,\"Location\":{\"x\":4,"
        "\"y\":1},\"ValueFunction\":\"logn\",\"Target\":{\"x\":4,\"y\":1}}],"
        "\"TargetDeck\":{\"Hit\":[{\"x\":4,\"y\":1}],\"Left\":[{\"x\":0,\"y\":"
        "0},{\"x\":0,\"y\":4}]},\"DiffPercentage\":0.0,\"Done\":false,"
        "\"HomeLocation\":{\"x\":4,\"y\":4},\"Size\":{\"x\":5,\"y\":"
        "5},\"Map\":[[1,0,0,0,1],[0,0,0,0,0],[0,0,0,0,0],[0,"
        "0,0,0,0],[0,0,0,0,0]]}";
  BOOST_CHECK(line == ans);
  state = State(file);
  simulation::solveState(state);
  state.writeOut(file);
  t = std::ifstream(file);
  std::getline(t, line);
  t.close();
  ans = "{\"Drones\":[{\"batteryLifeLeft\":96,\"Location\":{\"x\":3,"
        "\"y\":1},\"ValueFunction\":\"logn\",\"Target\":{\"x\":0,\"y\":0}}],"
        "\"TargetDeck\":{\"Hit\":[{\"x\":4,\"y\":1}],\"Left\":[{\"x\":0,\"y\":"
        "0},{\"x\":0,\"y\":4}]},\"DiffPercentage\":0.0,\"Done\":false,"
        "\"HomeLocation\":{\"x\":4,\"y\":4},\"Size\":{\"x\":5,\"y\":"
        "5},\"Map\":[[1,0,0,0,1],[0,0,0,0,0],[0,0,0,0,0],[0,"
        "0,0,0,0],[0,0,0,0,0]]}";
  BOOST_CHECK(line == ans);
}

BOOST_AUTO_TEST_SUITE_END()