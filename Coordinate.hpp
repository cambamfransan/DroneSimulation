#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <sstream>

struct Coordinate
{
  Coordinate() : x(0), y(0){};
  Coordinate(size_t x, size_t y) : x(x), y(y){};
  size_t x;
  size_t y;
};

#endif // !COORDINATE_HPP
