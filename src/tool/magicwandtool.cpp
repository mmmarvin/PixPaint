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
#include "magicwandtool.h"

#include <vector>
#include "../helper/selection_helpers.h"
#include "../image/image.h"
#include "../manager/selectionmanager.h"
#include "../utility/general_utility.h"
#include "../debug_log.h"

namespace pixpaint
{
namespace
{
  void setPixel(position_t x, position_t y, PixelData& layer, const Color& color)
  {
    std::memcpy(layer.getData() + (y * 4 * layer.getWidth()) + (x * 4), &color, sizeof(Color));
  }

  Color getPixel(position_t x, position_t y, const PixelData& layer)
  {
    return *reinterpret_cast<const Color*>(layer.getData() +
                                           (y * 4 * layer.getWidth()) +
                                           (x * 4));
  }
}
  MagicWandTool::MagicWandTool() :
    CursorToolBase("res/cursor/magic_wand_cursor.png", false)
  {
  }

  void MagicWandTool::onPreMousePress()
  {
    selection_helpers::tryFinalizeAllSelections(true);
  }

  bool MagicWandTool::onMousePress(const Point& currentPoint,
                                   const Point&,
                                   const Point&,
                                   const Color&,
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

    auto oc = currentLayer.getPixel(currentPoint.x, currentPoint.y);
    m_min = m_max = currentPoint;

    std::vector<Point> stack, points;
    stack.reserve(currentLayer.getWidth() * currentLayer.getHeight());
    points.reserve(currentLayer.getWidth() * currentLayer.getHeight());
    stack.push_back(currentPoint);
    points.push_back(currentPoint);

    PixelData color_map(currentLayer.getWidth(), currentLayer.getHeight(), Color::TRANSPARENT);
    position_t w = currentLayer.getWidth();
    position_t h = currentLayer.getHeight();
    while(!stack.empty()) {
      auto t = stack.back();
      auto tx = t.x;
      auto ty = t.y;
      stack.pop_back();

      auto nx = tx - 1;
      if(nx >= 0 &&
         getPixel(nx, ty, currentLayer) == oc &&
         getPixel(nx, ty, color_map) == Color::TRANSPARENT) {
        setPixel(nx, ty, color_map, Color::BLACK);
        points.push_back(Point(nx, ty));
        stack.push_back(Point(nx, ty));

        if(nx < m_min.x) {
          m_min.x = nx;
        }
      }
      nx = tx + 1;
      if(nx < w &&
         getPixel(nx, ty, currentLayer) == oc &&
         getPixel(nx, ty, color_map) == Color::TRANSPARENT) {
        setPixel(nx, ty, color_map, Color::BLACK);
        points.push_back(Point(nx, ty));
        stack.push_back(Point(nx, ty));

        if(nx > m_max.x) {
          m_max.x = nx;
        }
      }

      auto ny = ty - 1;
      if(ny >= 0 &&
         getPixel(tx, ny, currentLayer) == oc &&
         getPixel(tx, ny, color_map) == Color::TRANSPARENT) {
        setPixel(tx, ny, color_map, Color::BLACK);
        points.push_back(Point(tx, ny));
        stack.push_back(Point(tx, ny));

        if(ny < m_min.y) {
          m_min.y = ny;
        }
      }
      ny = ty + 1;
      if(ny < h &&
         getPixel(tx, ny, currentLayer) == oc &&
         getPixel(tx, ny, color_map) == Color::TRANSPARENT) {
        setPixel(tx, ny, color_map, Color::BLACK);
        points.push_back(Point(tx, ny));
        stack.push_back(Point(tx, ny));

        if(ny > m_max.y) {
          m_max.y = ny;
        }
      }
    }

    auto dim = m_max - m_min + Point(1, 1);
    MaskablePixelData layer(dim.x, dim.y, Color::TRANSPARENT);
    PixelData mask(dim.x, dim.y, Color::TRANSPARENT);
    auto* lp = reinterpret_cast<Color*>(layer.getData());
    auto* mp = reinterpret_cast<Color*>(mask.getData());
    auto* clp = reinterpret_cast<Color*>(currentLayer.getData());
    auto dw = dim.x;
    for(const auto& p : points) {
      auto ax = p.x - m_min.x;
      auto ay = p.y - m_min.y;

      *(lp + (ay * dw) + ax) = oc;
      *(mp + (ay * dw) + ax) = Color::WHITE;
      *(clp + (p.y * w) + p.x) = Color::TRANSPARENT;
    }

    auto& selection_manager = getSelectionManager();
    selection_manager.setSelectionRect(m_min.x, m_min.y, dim.x, dim.y);
    selection_manager.setSelectionLayer(std::move(layer), std::move(mask));

    return true;
  }

  IntRect MagicWandTool::getDrawRect() const
  {
    auto diff = m_max - m_min;
    return IntRect(m_min.x - 1,
                   m_min.y - 1,
                   general_utils::max(diff.x, 1) + 1,
                   general_utils::max(diff.y, 1) + 1);
  }

  std::string MagicWandTool::getDescription() const
  {
    return "Magic Wand Tool";
  }
}
