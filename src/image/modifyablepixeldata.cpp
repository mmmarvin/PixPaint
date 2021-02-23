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
#include "modifyablepixeldata.h"

namespace pixpaint
{
  ModifyablePixelData::ModifyablePixelData(dimension_t width, dimension_t height, const Color& color) :
    MaskablePixelData(width, height, color),
    m_modified(false)
  {
  }

  void ModifyablePixelData::clear(const Color& color)
  {
    MaskablePixelData::clear(color);
    m_modified = false;
  }

  void ModifyablePixelData::resize(dimension_t width,
                                   dimension_t height,
                                   const Color& color,
                                   EResizeMode mode)
  {
    MaskablePixelData::resize(width, height, color, mode);
    m_modified = true;
  }

  void ModifyablePixelData::setPixelSoft(position_t x, position_t y, const Color& color)
  {
    MaskablePixelData::setPixelSoft(x, y, color);
    m_modified = true;
  }

  void ModifyablePixelData::setPixel(position_t x, position_t y, const Color& color)
  {
    MaskablePixelData::setPixel(x, y, color);
    m_modified = true;
  }

  void ModifyablePixelData::setModifiedFlag(bool modified) noexcept
  {
    m_modified = modified;
  }

  bool ModifyablePixelData::isModified() const noexcept
  {
    return m_modified;
  }
}
