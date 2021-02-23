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
#ifndef SINGLETON_H
#define SINGLETON_H

namespace pixpaint
{
  template<class T>
  struct Singleton
  {
    static T& getInstance();
  };

  template<class T>
  class InitializableSingleton
  {
  public:
    template<class... Args>
    static void initialize(Args&&... args);
    static T& getInstance();

  private:
    static T** getInstanceImpl();
  };

  template<class T>
  class DynamicSingleton
  {
  public:
    template<class... Args>
    static void initialize(Args&&... args);
    static T& getInstance();

  private:
    static T** getInstanceImpl();
  };
}

#include "singleton.inl"

#endif // SINGLETON_H
