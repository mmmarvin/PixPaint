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
#ifndef POINT_H
#define POINT_H

#ifndef NDEBUG
#include <iosfwd>
#endif // NDEBUG
#include "../type.h"

namespace pixpaint
{
  struct Size;
  struct Point
  {
    Point() noexcept;
    Point(position_t x_, position_t y_) noexcept;
    Point(const Size& s) noexcept;

    position_t x;
    position_t y;
  };

  bool operator==(const Point& lhs, const Point& rhs) noexcept;
  bool operator!=(const Point& lhs, const Point& rhs) noexcept;

  Point& operator+=(Point& lhs, const Point& rhs);
  Point operator+(const Point& lhs, const Point& rhs);
  Point& operator-=(Point& lhs, const Point& rhs);
  Point operator-(const Point& lhs, const Point& rhs);

  Point& operator*=(Point& lhs, position_t rhs);
  Point operator*(const Point& lhs, position_t rhs);
  Point& operator/=(Point& lhs, position_t rhs);
  Point operator/(const Point& lhs, position_t rhs);

  position_t magnitude(const Point& p);
  position_t magnitude2(const Point& p);
  position_t length(const Point& p1, const Point& p2);
  Point normalize(const Point& p);
#ifndef NDEBUG
  std::ostream& operator<<(std::ostream& lhs, const pixpaint::Point& rhs);
  std::istream& operator>>(std::istream& lhs, pixpaint::Point& rhs);
#endif // NDEBUG
}

#endif // POINT_H
