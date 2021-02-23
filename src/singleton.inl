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
#include "singleton.h"

#include <cassert>
#include <utility>

namespace pixpaint
{
  template<class T>
  T& Singleton<T>::getInstance()
  {
    static T instance;
    return instance;
  }

  template<class T>
  template<class... Args>
  void InitializableSingleton<T>::initialize(Args&&... args)
  {
    static T instance(std::forward<Args>(args)...);
    *getInstanceImpl() = &instance;
  }

  template<class T>
  T& InitializableSingleton<T>::getInstance()
  {
    assert(*getInstanceImpl() != nullptr);
    return **getInstanceImpl();
  }

  template<class T>
  T** InitializableSingleton<T>::getInstanceImpl()
  {
    static T* instance = nullptr;
    return &instance;
  }

  template<class T>
  template<class... Args>
  void DynamicSingleton<T>::initialize(Args&&... args)
  {
    static T* instance = new T(std::forward<Args>(args)...);
    *getInstanceImpl() = instance;
  }

  template<class T>
  T& DynamicSingleton<T>::getInstance()
  {
    assert(*getInstanceImpl() != nullptr);
    return **getInstanceImpl();
  }

  template<class T>
  T** DynamicSingleton<T>::getInstanceImpl()
  {
    static T* instance = nullptr;
    return &instance;
  }
}
