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
#ifndef MASKABLEPIXELDATA_H
#define MASKABLEPIXELDATA_H

#include <memory>
#include "pixeldata.h"

namespace pixpaint
{
  class Mask;
  class MaskablePixelData : public PixelData
  {
  public:
    MaskablePixelData(position_t width, position_t height, const Color& color = Color::WHITE);
    MaskablePixelData(const PixelData& rhs);
    MaskablePixelData(PixelData&& rhs);

    void combine(const MaskablePixelData& pixelData, bool hard = false);
    void combine(const MaskablePixelData& pixelData, position_t x, position_t y, bool hard = false);

    void setMask(std::shared_ptr<Mask> mask);
    std::shared_ptr<Mask> getMask() const;
    void clearMask();
    bool isMaskSet() const;

    void setPixelSoft(position_t x, position_t y, const Color& color);
    void setPixel(position_t x, position_t y, const Color& color);

  private:
    std::shared_ptr<Mask> m_mask;
  };
}

#endif // MASKABLEPIXELDATA_H
