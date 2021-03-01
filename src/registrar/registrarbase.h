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
#include <unordered_set>
#include <vector>

namespace pixpaint
{
namespace registerbase_detail
{
  static constexpr auto NAME_SIZE_LIMIT = 64;
  static constexpr auto ID_SIZE_LIMIT = 32;
  static constexpr auto CATEGORY_SIZE_LIMIT = 64;
  static constexpr auto SHORTCUT_SIZE_LIMIT = 16;

  static constexpr auto FILE_EXTENSION_SIZE_LIMIT = 12;
  static constexpr auto FILE_DESCRIPTION_SIZE_LIMIT = 255;
}
  template<class T>
  class RegistrarBase
  {
    using id_container_type = std::unordered_set<std::string>;
    using container_type = std::vector<T>;

  public:
    RegistrarBase() {}

    template<class... Args> bool registerBuiltIn(std::string id, Args&&... args);
    template<class... Args> bool registerCustom(std::string id, Args&&... args);

    typename container_type::iterator begin();
    typename container_type::const_iterator begin() const;
    typename container_type::const_iterator cbegin() const;
    typename container_type::reverse_iterator rbegin();

    typename container_type::iterator end();
    typename container_type::const_iterator end() const;
    typename container_type::const_iterator cend() const;
    typename container_type::reverse_iterator rend();

    size_t size() const noexcept;

  private:
    id_container_type m_ids;
    container_type    m_items;
  };
}

#include "registrarbase.inl"

#endif // REGISTRARBASE_H
