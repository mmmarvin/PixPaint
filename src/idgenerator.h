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
#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <cinttypes>
#include <queue>

namespace pixpaint
{
  class IDGenerator
  {
  public:
    IDGenerator() {}
    void clear();

    void recycleID(std::size_t id);
    std::size_t getID();

    bool isThereID() const noexcept;
    std::size_t count() const noexcept;

  private:
    std::queue<std::size_t> m_ids;
  };
}

#endif // IDGENERATOR_H
