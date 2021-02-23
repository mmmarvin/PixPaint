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
#include "maskablepixeldata.h"

#include <cstring>
#include "../assert.h"
#include "mask.h"

namespace pixpaint
{
  MaskablePixelData::MaskablePixelData(position_t width, position_t height, const Color& color) :
    PixelData(width, height, color),
    m_mask(nullptr)
  {
  }

  MaskablePixelData::MaskablePixelData(const PixelData& rhs) :
    PixelData(rhs),
    m_mask(nullptr)
  {
  }

  MaskablePixelData::MaskablePixelData(PixelData&& rhs) :
    PixelData(std::move(rhs)),
    m_mask(nullptr)
  {
  }

  void MaskablePixelData::combine(const MaskablePixelData& pixelData, bool hard)
  {
    combine(pixelData, 0, 0, hard);
  }

  void MaskablePixelData::combine(const MaskablePixelData& pixelData, position_t x, position_t y, bool hard)
  {
    PixelData::combine(pixelData, x, y, hard,
    [mask = &m_mask](unsigned char* dst_ptr, const unsigned char* src_ptr, position_t i, position_t j) {
      if(mask->get()) {
        if(mask->get()->contains(i, j)) {
          std::memcpy(dst_ptr, src_ptr, 4);
        }
      } else {
        std::memcpy(dst_ptr, src_ptr, 4);
      }
    },
    [mask = &m_mask](unsigned char* dst_ptr, const unsigned char* src_ptr, position_t i, position_t j) {
      if(mask->get()) {
        if(mask->get()->contains(i, j)) {
          color_detail::alphaBlend(dst_ptr, src_ptr);
        }
      } else {
        color_detail::alphaBlend(dst_ptr, src_ptr);
      }
    });
  }

  void MaskablePixelData::setMask(std::shared_ptr<Mask> mask)
  {
    m_mask = std::move(mask);
  }

  std::shared_ptr<Mask> MaskablePixelData::getMask() const
  {
    return m_mask;
  }

  void MaskablePixelData::clearMask()
  {
    m_mask.reset();
  }

  bool MaskablePixelData::isMaskSet() const
  {
    return m_mask && !m_mask->isSet();
  }

  void MaskablePixelData::setPixelSoft(position_t x, position_t y, const Color& color)
  {
    if(m_mask && !m_mask->contains(x, y)) {
      return;
    }

    PixelData::setPixelSoft(x, y, color);
  }

  void MaskablePixelData::setPixel(position_t x, position_t y, const Color& color)
  {
    if(m_mask && !m_mask->contains(x, y)) {
      return;
    }

    PixelData::setPixel(x, y, color);
  }
}
