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
#include "general_utility.h"

#include <cmath>
#include "../assert.h"

namespace pixpaint
{
namespace general_utils
{
  template<class T>
  T max(T a, T b)
  {
    return a > b ? a : b;
  }

  template<class T>
  T min(T a, T b)
  {
    return a < b ? a : b;
  }

  template<class ContainerT>
  decltype(auto) minelement(ContainerT&& container)
  {
    PIXPAINT_ASSERT(container.size() != 0, "Container is empty");

    auto min = *container.begin();
    for(auto& e : container) {
      if(e < min) {
        min = e;
      }
    }

    return min;
  }

  template<class ContainerT>
  decltype(auto) maxelement(ContainerT&& container)
  {
    PIXPAINT_ASSERT(container.size() != 0, "Container is empty");

    auto max = *container.begin();
    for(auto& e : container) {
      if(e > max) {
        max = e;
      }
    }

    return max;
  }

  template<class T>
  T clamp(T value, T floor, T ceil)
  {
    return value > floor ? (value < ceil ? value : ceil) : floor;
  }

  template<class T>
  std::pair<T, T> minmax(T a, T b)
  {
    return a < b ? std::make_pair(a, b) : std::make_pair(b, a);
  }
  template<class T>
  std::uintptr_t pointerToInt(T* ptr)
  {
    return reinterpret_cast<std::intptr_t>(ptr);
  }

  template<class T>
  T fastAbs(T t)
  {
    return t < 0 ? -t : t;
  }

  template<class T>
  T fastMod(T value, T ceil)
  {
    return value > ceil ? std::fmod(value, ceil) : value;
  }

  template<class T>
  bool swapIfLess(T& t1, T& t2)
  {
    auto n = t1 < t2;
    if(n) {
      std::swap(t1, t2);
    }

    return n;
  }
}
}
