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
#include "ditherfilltool.h"

#include <stack>
#include "../image/color.h"
#include "../image/image.h"
#include "../image/maskablepixeldata.h"
#include "../image/point.h"
#include "../manager/drawermanager.h"
#include "../utility/general_utility.h"
#include "../manager/imagemanager.h"
#include "filltool.h"

namespace pixpaint
{
namespace
{
  class FillColorMap
  {
  public:
    bool isSet(position_t x, position_t y) const
    {
      auto find_it = m_colorMap.find(std::make_pair(x, y));
      if(find_it == m_colorMap.end()) {
        m_colorMap.emplace(std::make_pair(x, y));
        return false;
      }

      return true;
    }

  private:
    mutable ska::unordered_set<std::pair<position_t, position_t>, boost::hash<std::pair<position_t, position_t>>> m_colorMap;
  };
}
  DitherFillTool::DitherFillTool() :
    CursorToolBase("res/cursor/fill_cursor.png", false)
  {
  }

  // TODO: Faster algorithm?
  bool DitherFillTool::onMousePress(const Point& currentPoint,
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
    if(original_color == color ||
       currentLayer.getPixel(currentPoint.x - 1, currentPoint.y) == color ||
       currentLayer.getPixel(currentPoint.x + 1, currentPoint.y) == color ||
       currentLayer.getPixel(currentPoint.x, currentPoint.y - 1) == color ||
       currentLayer.getPixel(currentPoint.x, currentPoint.y + 1) == color) {
      m_min = m_max = Point(-1, -1);
      return false;
    }

    std::vector<std::pair<Point, Color>> h;
    h.reserve(currentLayer.getWidth() * currentLayer.getHeight());

    std::vector<std::pair<Point, bool>> s;
    s.reserve(currentLayer.getWidth() * currentLayer.getHeight());

    FillColorMap cm;

    currentLayer.setPixel(currentPoint.x, currentPoint.y, color);
    cm.isSet(currentPoint.x, currentPoint.y);
    h.push_back({ currentPoint, original_color });
    s.push_back({ currentPoint, true });

    int image_width = currentLayer.getWidth();
    int image_height = currentLayer.getHeight();
    m_min = m_max = currentPoint;
    while(!s.empty()) {
      auto t = s.back();
      auto p = std::get<0>(t);
      auto fill = std::get<1>(t);
      s.pop_back();

      if(fill) {
        // fill left
        auto np = p.x - 1;
        if(np >= 0 &&
           currentLayer.getPixel(np, p.y) == original_color &&
           !cm.isSet(np, p.y)) {
          s.push_back({ Point(np, p.y), !fill });
          m_min.x = general_utils::min(m_min.x, np);
        }

        // fill right
        np = p.x + 1;
        if(np < image_width &&
           currentLayer.getPixel(np, p.y) == original_color &&
           !cm.isSet(np, p.y)) {
          s.push_back({ Point(np, p.y), !fill });
          m_max.x = general_utils::max(m_max.x, np);
        }

        // fill top
        np = p.y - 1;
        if(np >= 0 &&
           currentLayer.getPixel(p.x, np) == original_color &&
           !cm.isSet(p.x, np)) {
          s.push_back({ Point(p.x, np), !fill });
          m_min.y = general_utils::min(m_min.y, np);
        }

        // fill bottom
        np = p.y + 1;
        if(np < image_height &&
           currentLayer.getPixel(p.x, np) == original_color &&
           !cm.isSet(p.x, np)) {
          s.push_back({ Point(p.x, np), !fill });
          m_max.y = general_utils::max(m_max.y, np);
        }
      } else {
        // fill left
        auto np = p.x - 1;
        if(np >= 0 &&
           currentLayer.getPixel(np, p.y) == original_color &&
           !cm.isSet(np, p.y)) {
          h.push_back({ Point(np, p.y), original_color });
          s.push_back({ Point(np, p.y), !fill });

          currentLayer.setPixel(np, p.y, color);

          m_min.x = general_utils::min(m_min.x, np);
        }

        // fill right
        np = p.x + 1;
        if(np < image_width &&
           currentLayer.getPixel(np, p.y) == original_color &&
           !cm.isSet(np, p.y)) {
          h.push_back({ Point(np, p.y), original_color });
          s.push_back({ Point(np, p.y), !fill });

          currentLayer.setPixel(np, p.y, color);

          m_max.x = general_utils::max(m_max.x, np);
        }

        // fill top
        np = p.y - 1;
        if(np >= 0 &&
           currentLayer.getPixel(p.x, np) == original_color &&
           !cm.isSet(p.x, np)) {
          h.push_back({ Point(p.x, np), original_color });
          s.push_back({ Point(p.x, np), !fill });

          currentLayer.setPixel(p.x, np, color);

          m_min.y = general_utils::min(m_min.y, np);
        }

        // fill bottom
        np = p.y + 1;
        if(np < image_height &&
           currentLayer.getPixel(p.x, np) == original_color &&
           !cm.isSet(p.x, np)) {
          h.push_back({ Point(p.x, np), original_color });
          s.push_back({ Point(p.x, np), !fill });

          currentLayer.setPixel(p.x, np, color);

          m_max.y = general_utils::max(m_max.y, np);
        }
      }
    }

    if(!h.empty()) {
      getLayerDrawer().getDrawerCommander()->addCommand(std::move(h));
    }
    getImageManager().getImage().setModified(true);
    return true;
  }

  bool DitherFillTool::hasRightClick() const
  {
    return true;
  }

  IntRect DitherFillTool::getDrawRect() const
  {
    auto diff = m_max - m_min;
    return IntRect(m_min.x - 1,
                   m_min.y - 1,
                   general_utils::max(diff.x, 1) + 1,
                   general_utils::max(diff.y, 1) + 1);
  }

  std::string DitherFillTool::getDescription() const
  {
    return "Dither Fill Tool";
  }
}
