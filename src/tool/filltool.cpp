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
#include "filltool.h"

#include <stack>
#include "../image/color.h"
#include "../image/image.h"
#include "../image/maskablepixeldata.h"
#include "../image/mask.h"
#include "../image/point.h"
#include "../manager/drawermanager.h"
#include "../manager/imagemanager.h"
#include "../utility/general_utility.h"

namespace pixpaint
{
namespace
{
  /*
   * A Seed Fill Algorithm
   * by Paul Heckbert
   * from "Graphics Gems", Academic Press, 1990
   *
   * user provides pixelread() and pixelwrite() routines
   */

  /*
   * fill.c : simple seed fill program
   * Calls pixelread() to read pixels, pixelwrite() to write pixels.
   *
   * Paul Heckbert	13 Sept 1982, 28 Jan 1987
   */
  struct Segment
  {
    short y, xl, xr, dy;
  };

  struct Dimension
  {
    int x0;
    int x1;
    int y0;
    int y1;
  };

#define MAX 10000		/* max depth of stack */

#define PUSH(Y, XL, XR, DY)	/* push new segment on stack */ \
    if (sp < stack + MAX && Y + (DY) >= dim.y0 && Y + (DY) <= dim.y1) \
    { sp->y = Y; sp->xl = XL; sp->xr = XR; sp->dy = DY; sp++;}

#define POP(Y, XL, XR, DY)	/* pop segment off stack */ \
    {sp--; Y = sp->y + (DY = sp->dy); XL = sp->xl; XR = sp->xr;}

  // Layer setPixel and getPixel have checkers that could cause slow-downs
  // when used in fill
  void set_pixel(int x, int y, const Color& color, MaskablePixelData& layer)
  {
    auto mask = layer.getMask();
    if(mask) {
      if(mask->contains(x, y)) {
        auto* c = layer.getData() + (y * 4 * layer.getWidth()) + (x * 4);
        std::memcpy(c, &color, sizeof(Color));
      }
    } else {
      auto* c = layer.getData() + (y * 4 * layer.getWidth()) + (x * 4);
      std::memcpy(c, &color, sizeof(Color));
    }
  }

  Color get_pixel(int x, int y, MaskablePixelData& layer)
  {
    Color ret;
    std::memcpy(&ret, layer.getData() + (y * 4 * layer.getWidth()) + (x * 4), sizeof(Color));
    return ret;
  }

  /*
   * fill: set the pixel at (x,y) and all of its 4-connected neighbors
   * with the same pixel value to the new pixel value nv.
   * A 4-connected neighbor is a pixel above, below, left, or right of a pixel.
   */
  std::vector<std::pair<Point, Color>> fill(int x, int y, const Color& nv, MaskablePixelData& layer)
  {
    std::vector<std::pair<Point, Color>> history;
    if(!layer.getWidth() || !layer.getHeight()) {
      return history;
    }

    auto mask = layer.getMask();
    if(mask && !mask->contains(x, y)) {
      return history;
    }
    history.reserve(layer.getWidth() * layer.getHeight());

    Dimension dim = { 0, layer.getWidth() - 1, 0, layer.getHeight() - 1 };

    int l, x1, x2, dy;
    Color ov;	/* old pixel value */
    Segment stack[MAX], *sp = stack;	/* stack of filled segments */

    ov = get_pixel(x, y, layer);		/* read pv at seed point */
    if (ov == nv || x < dim.x0 || x > dim.x1 || y < dim.y0 || y > dim.y1) {
      return history;
    }
    PUSH(y, x, x, 1);			/* needed in some cases */
    PUSH(y + 1, x, x, -1);		/* seed segment (popped 1st) */

    while (sp>stack) {
      /* pop segment off stack and fill a neighboring scan line */
      POP(y, x1, x2, dy);
      /*
      * segment of scan line y-dy for x1<=x<=x2 was previously filled,
      * now explore adjacent pixels in scan line y
      */
      for (x = x1; x >= dim.x0 && get_pixel(x, y, layer) == ov; x--) {
        history.push_back(std::make_pair(Point(x, y), ov));
        set_pixel(x, y, nv, layer);
      }
      if (x >= x1) goto skip;
      l = x + 1;
      if (l < x1) PUSH(y, l, x1 - 1, -dy);		/* leak on left? */
      x = x1 + 1;
      do {
        for (; x <= dim.x1 && get_pixel(x, y, layer) == ov; x++) {
          history.push_back(std::make_pair(Point(x, y), ov));
          set_pixel(x, y, nv, layer);
        }
        PUSH(y, l, x - 1, dy);
        if (x > x2 + 1) PUSH(y, x2 + 1, x - 1, -dy);	/* leak on right? */
    skip:
        for (x++; x <= x2 && get_pixel(x, y, layer) != ov; x++);
        l = x;
      } while (x <= x2);
    }

    return history;
  }
}
  FillTool::FillTool() :
    CursorToolBase("res/cursor/fill_cursor.png", false)
  {
  }

  bool FillTool::onMousePress(const Point& currentPoint,
                              const Point&,
                              const Point&,
                              const Color& color,
                              ControlState,
                              ModifyablePixelData&,
                              MaskablePixelData& currentLayer)
  {
    if(currentPoint.x < 0 || currentPoint.x >= currentLayer.getWidth() ||
       currentPoint.y < 0 || currentPoint.y >= currentLayer.getHeight() ||
       currentLayer.getWidth() == 0 || currentLayer.getHeight() == 0) {
      m_min = m_max = Point(-1, -1);
      return false;
    }

    auto original_color = currentLayer.getPixel(currentPoint.x, currentPoint.y);
    if(original_color == color) {
      m_min = m_max = Point(-1, -1);
      return false;
    }

    auto points = fill(currentPoint.x, currentPoint.y, color, currentLayer);
    m_min = Point(0, 0);
    m_max = Point(currentLayer.getWidth(), currentLayer.getHeight());

    if(!points.empty()) {
      getLayerDrawer().getDrawerCommander()->addCommand(std::move(points));
    }
    getImageManager().getImage().setModified(true);

    return true;
  }

  bool FillTool::hasRightClick() const
  {
    return true;
  }

  IntRect FillTool::getDrawRect() const
  {
    auto diff = m_max - m_min;
    return IntRect(m_min.x - 1,
                   m_min.y - 1,
                   general_utils::max(diff.x, 1) + 1,
                   general_utils::max(diff.y, 1) + 1);
  }

  std::string FillTool::getDescription() const
  {
    return "Fill Tool";
  }
}
