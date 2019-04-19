cmake_minimum_required(VERSION 3.12.0)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH
  "Locaction for installation"
  FORCE)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build, options are: Debug Release"
  FORCE)

project(Drones)
set(externpro_REV 18.11.1)
find_package(externpro REQUIRED)
xpSetFlags()
set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(lib_name DronesLib)

set(${lib_name}_srcs
  Configuration.cpp
  Configuration.hpp
  Coordinate.hpp
  SimulationImpl.cpp
  SimulationImpl.hpp
  Simulation.cpp
  Simulation.hpp
  State.cpp
  State.hpp
  Parallel.cpp
  Parallel.hpp
  )

source_group("" FILES ${${lib_name}_srcs})

add_library(${lib_name} ${${lib_name}_srcs})
xpTargetExtern(${lib_name} PUBLIC boost rapidjson)

set(exe_name Drones)

set(${exe_name}_srcs
  Main.cpp
  )

source_group("" FILES ${${exe_name}_srcs})

add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries( ${exe_name} ${lib_name} )
install(TARGETS ${exe_name} DESTINATION bin)

enable_testing()
set(test_exe TestDrones)

set(${test_exe}_srcs
  ConfigurationTest.cpp
  SimulationTest.cpp
  TestDrones.cpp
  )

source_group("" FILES ${${test_exe}_srcs})
add_executable(${test_exe} ${${test_exe}_srcs})
target_link_libraries( ${test_exe} ${lib_name} )
add_test(NAME ${test_exe} COMMAND ${test_exe})
