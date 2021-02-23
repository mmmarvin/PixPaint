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
#ifndef MODIFYABLEPIXELDATA_H
#define MODIFYABLEPIXELDATA_H

#include "maskablepixeldata.h"

namespace pixpaint
{
  class ModifyablePixelData : public MaskablePixelData
  {
  public:
    ModifyablePixelData(dimension_t width, dimension_t height, const Color& color = Color::WHITE);
    void clear(const Color& color = Color::WHITE);

    void resize(dimension_t width,
                dimension_t height,
                const Color& color = Color::WHITE,
                EResizeMode mode = EResizeMode::RESIZE);

    void setPixelSoft(position_t x, position_t y, const Color& color);
    void setPixel(position_t x, position_t y, const Color& color);

    void setModifiedFlag(bool modified) noexcept;
    bool isModified() const noexcept;

  private:
    bool m_modified;
  };
}

#endif // MODIFYABLEPIXELDATA_H
