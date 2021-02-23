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
#ifndef SIZE_H
#define SIZE_H

#include "../type.h"

namespace pixpaint
{
  struct Point;
  struct Size
  {
    Size() noexcept;
    Size(dimension_t width_, dimension_t height_) noexcept;
    Size(const Point& p) noexcept;

    dimension_t width;
    dimension_t height;
  };

  bool operator==(const Size& lhs, const Size& rhs) noexcept;
  bool operator!=(const Size& lhs, const Size& rhs) noexcept;

  Size& operator+=(Size& lhs, const Size& rhs);
  Size operator+(const Size& lhs, const Size& rhs);
  Size& operator-=(Size& lhs, const Size& rhs);
  Size operator-(const Size& lhs, const Size& rhs);

  Size& operator*=(Size& lhs, dimension_t rhs);
  Size operator*(const Size& lhs, dimension_t rhs);
  Size& operator/=(Size& lhs, dimension_t rhs);
  Size operator/(const Size& lhs, dimension_t rhs);
}

#endif // SIZE_H
