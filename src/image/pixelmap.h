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
#ifndef PIXELMAP_H
#define PIXELMAP_H

#include <boost/unordered_map.hpp>
#include "../type.h"
#include "color.h"

namespace pixpaint
{
  class PixelMap
  {
  public:
    PixelMap() {}
    void clear();

    void setPixel(position_t x, position_t y, const Color& color);
    Color getPixel(position_t x, position_t y) const;

  private:
    friend class PixelData;
    friend class MaskablePixelData;

    boost::unordered_map<std::pair<position_t, position_t>, Color> m_pixels;
  };
}

#endif // PIXELMAP_H
