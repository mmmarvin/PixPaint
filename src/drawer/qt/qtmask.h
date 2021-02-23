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
#ifndef QTMASK_H
#define QTMASK_H

#include <QRegion>
#include "../../image/mask.h"

namespace pixpaint
{
  class QtMask : public Mask
  {
  public:
    QtMask(const QRegion& region);

    bool contains(position_t x, position_t y) const override;
    bool contains(const Point& p) const override;
    bool isSet() const override;

    QRegion& getRegion() noexcept;
    const QRegion& getRegion() const noexcept;

  private:
    QRegion m_region;
  };
}

#endif // QTMASK_H
