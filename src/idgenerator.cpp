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
#include "idgenerator.h"

namespace pixpaint
{
  void IDGenerator::clear()
  {
    while(!m_ids.empty()) {
      m_ids.pop();
    }
  }

  void IDGenerator::recycleID(std::size_t id)
  {
    m_ids.push(id);
  }

  std::size_t IDGenerator::getID()
  {
    auto id = m_ids.front();
    m_ids.pop();

    return id;
  }

  bool IDGenerator::isThereID() const noexcept
  {
    return !m_ids.empty();
  }

  std::size_t IDGenerator::count() const noexcept
  {
    return m_ids.size();
  }
}
