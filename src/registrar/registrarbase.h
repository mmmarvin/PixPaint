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
#ifndef REGISTRARBASE_H
#define REGISTRARBASE_H

#include <memory>
#include <vector>

namespace pixpaint
{
  template<class T>
  class RegistrarBase
  {
    using container_type = std::vector<T>;

  public:
    RegistrarBase() {}

    template<class... Args> void registerBuiltIn(Args&&... args);
    template<class... Args> void registerCustom(Args&&... args);

    typename container_type::iterator begin();
    typename container_type::const_iterator begin() const;
    typename container_type::const_iterator cbegin() const;
    typename container_type::reverse_iterator rbegin();

    typename container_type::iterator end();
    typename container_type::const_iterator end() const;
    typename container_type::const_iterator cend() const;
    typename container_type::reverse_iterator rend();

  private:
    container_type m_items;
  };
}

#include "registrarbase.inl"

#endif // REGISTRARBASE_H
