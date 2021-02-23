/**********
 *
 *     PixPaint
 *     Copyright 2020-2021 Marvin Manese
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 **********/
#include "point.h"

#include <cmath>
#ifndef NDEBUG
#include <iostream>
#endif // NDEBUG
#include <string>
#include "size.h"

namespace pixpaint
{
  Point::Point() noexcept :
    x(0),
    y(0)
  {
  }

  Point::Point(position_t x_, position_t y_) noexcept :
    x(x_),
    y(y_)
  {
  }

  Point::Point(const Size& s) noexcept :
    x(s.width),
    y(s.height)
  {
  }

  bool operator==(const Point& lhs, const Point& rhs) noexcept
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  bool operator!=(const Point& lhs, const Point& rhs) noexcept
  {
    return !(lhs == rhs);
  }

  Point& operator+=(Point& lhs, const Point& rhs)
  {
    lhs.x += rhs.x;
    lhs.y += rhs.y;

    return lhs;
  }

  Point operator+(const Point& lhs, const Point& rhs)
  {
    auto temp = lhs;
    temp += rhs;

    return temp;
  }

  Point& operator-=(Point& lhs, const Point& rhs)
  {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;

    return lhs;
  }

  Point operator-(const Point& lhs, const Point& rhs)
  {
    auto temp = lhs;
    temp -= rhs;

    return temp;
  }

  Point& operator*=(Point& lhs, position_t rhs)
  {
    lhs.x *= rhs;
    lhs.y *= rhs;

    return lhs;
  }

  Point operator*(const Point& lhs, position_t rhs)
  {
    auto temp = lhs;
    temp *= rhs;

    return temp;
  }

  Point& operator/=(Point& lhs, position_t rhs)
  {
    lhs.x /= rhs;
    lhs.y /= rhs;

    return lhs;
  }

  Point operator/(const Point& lhs, position_t rhs)
  {
    auto temp = lhs;
    temp /= rhs;

    return temp;
  }

  position_t magnitude(const Point& p)
  {
    return std::sqrt(magnitude2(p));
  }

  position_t magnitude2(const Point& p)
  {
    auto vx = p.x;
    auto vy = p.y;

    return (vx * vx) + (vy * vy);
  }

  position_t length(const Point& p1, const Point& p2)
  {
    auto nx = p2.x - p1.x;
    auto ny = p2.y - p1.y;

    return std::sqrt((nx * nx) + (ny * ny));
  }

  Point normalize(const Point& p)
  {
    auto length = magnitude(p);
    if(length > 0) {
        return p / length;;
    }

    return p;
  }
#ifndef NDEBUG
  std::ostream& operator<<(std::ostream& lhs, const pixpaint::Point& rhs)
  {
    return lhs << "[" << rhs.x << ", " << rhs.y << "]";
  }

  std::istream& operator>>(std::istream& lhs, pixpaint::Point& rhs)
  {
    return lhs >> rhs.x >> rhs.y;
  }
#endif // NDEBUG
}
