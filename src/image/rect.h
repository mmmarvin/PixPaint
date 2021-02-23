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
#ifndef RECT_H
#define RECT_H

#include "../image/point.h"
#include "../type.h"

namespace pixpaint
{
  template<class T>
  struct Rect
  {
    Rect() noexcept;
    Rect(T x_, T y_, T width_, T height_) noexcept;

    T x;
    T y;
    T width;
    T height;
  };

  template<class S, class T> Rect<S> castTo(const Rect<T>& rect);

  template<class T> Rect<T>& operator+=(Rect<T>& lhs, const Point& rhs);
  template<class T> Rect<T> operator+(const Rect<T>& lhs, const Point& rhs);
  template<class T> Rect<T>& operator-=(Rect<T>& lhs, const Point& rhs);
  template<class T> Rect<T> operator-(const Rect<T>& lhs, const Point& rhs);
  template<class T> Rect<T>& operator+=(Rect<T>& lhs, const Rect<T>& rhs);
  template<class T> Rect<T> operator+(const Rect<T>& lhs, const Rect<T>& rhs);
  template<class T> Rect<T>& operator-=(Rect<T>& lhs, const Rect<T>& rhs);
  template<class T> Rect<T> operator-(const Rect<T>& lhs, const Rect<T>& rhs);

  template<class T> Rect<T>& operator*=(Rect<T>& lhs, T rhs);
  template<class T> Rect<T> operator*(const Rect<T>& lhs, T rhs);
  template<class T> Rect<T>& operator/=(Rect<T>& lhs, T rhs);
  template<class T> Rect<T> operator/(const Rect<T>& lhs, T rhs);

  template<class T> bool contains(const Rect<T>& rect, const Point& point);
  template<class T> Point position(const Rect<T>& rect);
  template<class T> Point dimension(const Rect<T>& rect);
  template<class T> Point center(const Rect<T>& rect);

  template<class T> Point mapTo(const Rect<T>& rect, const Point& point);
  template<class T> Point mapFrom(const Rect<T>& rect, const Point& point);

  using IntRect = Rect<position_t>;
  using FloatRect = Rect<float>;
  using DoubleRect = Rect<double>;
}

#include "rect.inl"

#endif // RECT_H
