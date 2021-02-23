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
#include "size.h"

#include "point.h"

namespace pixpaint
{
  Size::Size() noexcept :
    width(0),
    height(0)
  {
  }

  Size::Size(dimension_t width_, dimension_t height_) noexcept :
    width(width_),
    height(height_)
  {
  }

  Size::Size(const Point& p) noexcept :
    width(p.x),
    height(p.y)
  {
  }

  bool operator==(const Size& lhs, const Size& rhs) noexcept
  {
    return lhs.width == rhs.width && lhs.height == rhs.height;
  }

  bool operator!=(const Size& lhs, const Size& rhs) noexcept
  {
    return !(lhs == rhs);
  }

  Size& operator+=(Size& lhs, const Size& rhs)
  {
    lhs.width += rhs.width;
    lhs.height += rhs.height;

    return lhs;
  }

  Size operator+(const Size& lhs, const Size& rhs)
  {
    auto temp = lhs;
    temp += rhs;

    return temp;
  }

  Size& operator-=(Size& lhs, const Size& rhs)
  {
    lhs.width -= rhs.width;
    lhs.height -= rhs.height;

    return lhs;
  }

  Size operator-(const Size& lhs, const Size& rhs)
  {
    auto temp = lhs;
    temp -= rhs;

    return temp;
  }

  Size& operator*=(Size& lhs, dimension_t rhs)
  {
    lhs.width *= rhs;
    lhs.height *= rhs;

    return lhs;
  }

  Size operator*(const Size& lhs, dimension_t rhs)
  {
    auto temp = lhs;
    temp *= rhs;

    return temp;
  }

  Size& operator/=(Size& lhs, dimension_t rhs)
  {
    lhs.width /= rhs;
    lhs.height /= rhs;

    return lhs;
  }

  Size operator/(const Size& lhs, dimension_t rhs)
  {
    auto temp = lhs;
    temp /= rhs;

    return temp;
  }
}
