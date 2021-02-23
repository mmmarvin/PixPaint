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
#include "pixelmap.h"

namespace pixpaint
{
  void PixelMap::clear()
  {
    m_pixels.clear();
  }

  void PixelMap::setPixel(position_t x, position_t y, const Color& color)
  {
    std::pair<position_t, position_t> posPair(x, y);

    auto findIt = m_pixels.find(posPair);
    if(findIt != m_pixels.end()) {
      findIt->second = color;
    } else {
      m_pixels.emplace(posPair, color);
    }
  }

  Color PixelMap::getPixel(position_t x, position_t y) const
  {
    auto findIt = m_pixels.find(std::make_pair(x, y));
    if(findIt != m_pixels.end()) {
      return findIt->second;
    }

    return Color::TRANSPARENT;
  }
}
