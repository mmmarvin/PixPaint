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
#include "rect.h"

namespace pixpaint
{
  template<class T>
  Rect<T>::Rect() noexcept :
    x(0),
    y(0),
    width(0),
    height(0)
  {
  }

  template<class T>
  Rect<T>::Rect(T x_, T y_, T width_, T height_) noexcept :
    x(x_),
    y(y_),
    width(width_),
    height(height_)
  {
  }

  template<class S, class T>
  Rect<S> castTo(const Rect<T>& rect)
  {
    return Rect<S>(rect.x, rect.y, rect.width, rect.height);
  }

  template<class T>
  Rect<T>& operator+=(Rect<T>& lhs, const Point& rhs)
  {
    lhs.x += rhs.x;
    lhs.y += rhs.y;

    return lhs;
  }

  template<class T>
  Rect<T> operator+(const Rect<T>& lhs, const Point& rhs)
  {
    auto temp = lhs;
    temp += rhs;

    return temp;
  }

  template<class T>
  Rect<T>& operator-=(Rect<T>& lhs, const Point& rhs)
  {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;

    return lhs;
  }

  template<class T>
  Rect<T> operator-(const Rect<T>& lhs, const Point& rhs)
  {
    auto temp = lhs;
    temp -= rhs;

    return temp;
  }

  template<class T>
  Rect<T>& operator+=(Rect<T>& lhs, const Rect<T>& rhs)
  {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.width += rhs.width;
    lhs.height += rhs.height;

    return lhs;
  }

  template<class T>
  Rect<T> operator+(const Rect<T>& lhs, const Rect<T>& rhs)
  {
    auto temp = lhs;
    temp += rhs;

    return lhs;
  }

  template<class T>
  Rect<T>& operator-=(Rect<T>& lhs, const Rect<T>& rhs)
  {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.width -= rhs.width;
    lhs.height -= rhs.height;

    return lhs;
  }

  template<class T>
  Rect<T> operator-(const Rect<T>& lhs, const Rect<T>& rhs)
  {
    auto temp = lhs;
    temp -= rhs;

    return temp;
  }

  template<class T>
  Rect<T>& operator*=(Rect<T>& lhs, T rhs)
  {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.width *= rhs;
    lhs.height *= rhs;

    return lhs;
  }

  template<class T>
  Rect<T> operator*(const Rect<T>& lhs, T rhs)
  {
    auto temp = lhs;
    temp *= rhs;

    return temp;
  }

  template<class T>
  Rect<T>& operator/=(Rect<T>& lhs, T rhs)
  {
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.width /= rhs;
    lhs.height /= rhs;

    return lhs;
  }

  template<class T>
  Rect<T> operator/(const Rect<T>& lhs, T rhs)
  {
    auto temp = lhs;
    temp /= rhs;

    return temp;
  }

  template<class T>
  bool contains(const Rect<T>& rect, const Point& point)
  {
    auto min = Point(rect.x, rect.y);
    auto max = Point(rect.x + rect.width, rect.y + rect.height);

    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y;
  }

  template<class T>
  Point position(const Rect<T>& rect)
  {
    return Point(rect.x, rect.y);
  }

  template<class T>
  Point dimension(const Rect<T>& rect)
  {
    return Point(rect.width, rect.height);
  }

  template<class T>
  Point center(const Rect<T>& rect)
  {
    T hw = (rect.width / 2), hh = (rect.height / 2);
    return position(rect) + Point(hw, hh);
  }

  template<class T>
  Point mapTo(const Rect<T>& rect, const Point& point)
  {
    return point - position(rect);
  }

  template<class T>
  Point mapFrom(const Rect<T>& rect, const Point& point)
  {
    return point + position(rect);
  }
}
