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
#include "geometry_utility.h"

#include <cmath>
#include "../image/maskablepixeldata.h"
#include "../image/pixeldata.h"
#include "../type.h"

namespace pixpaint
{
namespace geometry_utils
{
namespace
{
  void draw_line_x(const PixelData& brush, MaskablePixelData& layer, position_t x1, position_t y1, position_t x2, position_t y2, bool hard)
  {
    auto dx = x2 - x1;
    auto dy = y2 - y1;
    auto yi = 1;
    if(dy < 0) {
      yi = -yi;
      dy = -dy;
    }

    auto e = (2 * dy) - dx;
    auto y = y1;

    for(position_t x = x1; x <= x2; ++x) {
      layer.combine(brush, x, y, hard);

      if(e > 0) {
        y += yi;
        e += 2 * (dy - dx);
      } else {
        e += 2 * dy;
      }
    }
  }

  void draw_line_y(const PixelData& brush, MaskablePixelData& layer, position_t x1, position_t y1, position_t x2, position_t y2, bool hard)
  {
    auto dx = x2 - x1;
    auto dy = y2 - y1;
    auto xi = 1;
    if(dx < 0) {
      xi = -xi;
      dx = -dx;
    }

    auto e = (2 * dx) - dy;
    auto x = x1;

    for(position_t y = y1; y < y2; ++y) {
      layer.combine(brush, x, y, hard);
      if(e > 0) {
        x += xi;
        e += 2 * (dx - dy);
      } else {
        e += 2 * dx;
      }
    }
  }
}
  // Bresenham's line algorithm
  void drawLine(const PixelData& brush, MaskablePixelData& layer, const Point& p1, const Point& p2, bool hard)
  {
    position_t hw = brush.getWidth() / 2;
    position_t hh = brush.getHeight() / 2;

    auto x1 = p1.x - hw;
    auto y1 = p1.y - hh;
    auto x2 = p2.x - hw;
    auto y2 = p2.y - hh;
    if(abs(y2 - y1) < abs(x2 - x1)) {
      if(x1 > x2) {
        draw_line_x(brush, layer, x2, y2, x1, y1, hard);
      } else {
        draw_line_x(brush, layer, x1, y1, x2, y2, hard);
      }
    } else {
      if(y1 > y2) {
        draw_line_y(brush, layer, x2, y2, x1, y1, hard);
      } else {
        draw_line_y(brush, layer, x1, y1, x2, y2, hard);
      }
    }
  }
}
}
