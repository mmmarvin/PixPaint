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
#ifndef GENERAL_UTILITY_H
#define GENERAL_UTILITY_H

#include <cinttypes>
#include <tuple>

namespace pixpaint
{
namespace general_utils
{
  template<class T>
  T max(T a, T b);

  template<class T>
  T min(T a, T b);

  template<class ContainerT>
  decltype(auto) minelement(ContainerT&& container);

  template<class ContainerT>
  decltype(auto) maxelement(ContainerT&& container);

  template<class T>
  T clamp(T value, T floor, T ceil);

  template<class T>
  std::pair<T, T> minmax(T a, T b);

  template<class T>
  std::uintptr_t pointerToInt(T* ptr);

  template<class T>
  T fastAbs(T t);

  template<class T>
  T fastMod(T value, T ceil);

  template<class T>
  bool swapIfLess(T& t1, T& t2);
}
}

#include "general_utility.inl"

#endif // GENERAL_UTILITY_H
