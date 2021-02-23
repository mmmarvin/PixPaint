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
#include "qtmask.h"

#include "../../utility/qt_utility.h"

namespace pixpaint
{
  QtMask::QtMask(const QRegion& region) :
    m_region(region)
  {
  }

  bool QtMask::contains(position_t x, position_t y) const
  {
    return m_region.contains(QPoint(x, y));
  }

  bool QtMask::contains(const Point& p) const
  {
    return m_region.contains(qt_utils::convertToQTPoint(p)) ;
  }

  bool QtMask::isSet() const
  {
    return !m_region.isEmpty();
  }

  QRegion& QtMask::getRegion() noexcept
  {
    return m_region;
  }

  const QRegion& QtMask::getRegion() const noexcept
  {
    return m_region;
  }
}
