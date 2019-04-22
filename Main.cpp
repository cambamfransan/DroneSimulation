#include <iostream>
#include <map>
#include <string>

#include <boost/filesystem.hpp>

#include "Configuration.hpp"
#include "Parallel.hpp"
#include "SimulationImpl.hpp"

int main(int argc, char** argv)
{
  if (argc < 3 || (std::strcmp(argv[1], "solve") != 0 &&
                   std::strcmp(argv[1], "solveState") != 0))
  {
    std::cout
      << "There are two ways to run this executable:" << std::endl
      << "solve <config path> will solve the path with all of the "
         "input configurations"
      << std::endl
      << "solveState <state path> with solve the next state from the state "
         "that was given. The solved state will be in same location"
      << std::endl;

    return EXIT_FAILURE;
  }

  if (!std::strcmp(argv[1], "solve"))
  {
    if (argc != 4)
    {
      std::cout
        << "To solve a configuration, run ./Drones solve <input> <output>"
        << std::endl;
      return EXIT_FAILURE;
    }
    if (!boost::filesystem::exists(argv[2]) ||
        !boost::filesystem::exists(
          boost::filesystem::path(argv[3]).parent_path()))
    {
      std::cout << "please give a path to a config file that exists"
                << std::endl;
      return EXIT_FAILURE;
    }
    Configuration config(argv[2]);
    RunConfig run(config);
    run.startTasks(argv[3]);
  }
  else if (!std::strcmp(argv[1], "solveState"))
  {
    if (!boost::filesystem::exists(argv[2]))
    {
      std::cout << "please give a path to a config file that exists"
                << std::endl;
      return EXIT_FAILURE;
    }
    State state(argv[2]);
    simulation::solveState(state);
    state.writeOut(argv[2]);
  }

  return EXIT_SUCCESS;
}