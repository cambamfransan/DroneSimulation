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

inline bool operator==(const Coordinate& lhs, const Coordinate& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Coordinate& lhs, const Coordinate& rhs)
{
  return !(lhs == rhs);
}

#endif // !COORDINATE_HPP
