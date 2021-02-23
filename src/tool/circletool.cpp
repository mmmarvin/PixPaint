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
#include "circletool.h"

#include "../helper/tool_helpers.h"
#include "../image/color.h"
#include "../image/image.h"
#include "../image/maskablepixeldata.h"
#include "../image/modifyablepixeldata.h"
#include "../manager/drawermanager.h"
#include "../manager/imagemanager.h"
#include "../utility/general_utility.h"
#include "../define.h"

namespace pixpaint
{
  static Drawer* currentDrawer = nullptr;

  CircleTool::CircleTool() :
    CursorToolBase(Qt::CrossCursor),
    m_min(-1, -1),
    m_max(1, 1),
    m_size(1),
    m_antialiased(DEFAULT_TOOL_ANTIALIASING),
    m_fill(true)
  {
    this->addIntegerValueOption(&m_size, "Size", 1, 100);
    this->addFlagOption(&m_fill, "Fill Color");
    this->addFlagOption(&m_antialiased, "Anti-Aliased");
  }

  bool CircleTool::onMousePress(const Point& currentPoint,
                                const Point&,
                                const Point&,
                                const Color&,
                                ControlState controlState,
                                ModifyablePixelData& previewLayer,
                                MaskablePixelData&)
  {
    m_startPoint = m_endPoint = currentPoint;
    m_min = m_max = currentPoint;

    currentDrawer = &getPreviewDrawer();
    previewLayer.clear(Color::TRANSPARENT);
    tool_helpers::drawShape(currentDrawer,
                            &Drawer::drawCircleBlend,
                            controlState,
                            m_startPoint,
                            m_endPoint,
                            m_finalStartPoint,
                            m_finalEndPoint,
                            m_min,
                            m_max,
                            m_fill,
                            DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });

    return true;
  }

  bool CircleTool::onMouseMove(const Point& currentPoint,
                               const Point&,
                               const Point&,
                               const Color&,
                               ControlState controlState,
                               ModifyablePixelData& previewLayer,
                               MaskablePixelData&)
  {
    m_endPoint = currentPoint;

    previewLayer.clear(Color::TRANSPARENT);
    tool_helpers::drawShape(currentDrawer,
                            &Drawer::drawCircleBlend,
                            controlState,
                            m_startPoint,
                            m_endPoint,
                            m_finalStartPoint,
                            m_finalEndPoint,
                            m_min,
                            m_max,
                            m_fill,
                            DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });

    return true;
  }

  bool CircleTool::onMouseRelease(const Point&,
                                  const Point&,
                                  const Point&,
                                  const Color&,
                                  ControlState controlState,
                                  ModifyablePixelData& previewLayer,
                                  MaskablePixelData&)
  {
    previewLayer.clear(Color::TRANSPARENT);
    tool_helpers::drawShape(&getLayerDrawer(),
                            &Drawer::drawCircleBlend,
                            controlState,
                            m_startPoint,
                            m_endPoint,
                            m_finalStartPoint,
                            m_finalEndPoint,
                            m_min,
                            m_max,
                            m_fill,
                            DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });

    getImageManager().getImage().setModified(true);
    return true;
  }

  IntRect CircleTool::getDrawRect() const
  {
    // FIXME: Redraw rect doesn't work when shift is held down, and
    // the rect is created on the 1st quadrant (top-left quadrant)
    auto min = m_min;
    auto max = m_max;

    min.x -= m_size;
    min.y -= m_size;
    max.x += m_size;
    max.y += m_size;

    auto diff = max - min;
    return IntRect(min.x - 1,
                   min.y - 1,
                   general_utils::max(diff.x, 1) + 1,
                   general_utils::max(diff.y, 1) + 1);
  }

  optional<IntRect> CircleTool::getHistoryRect() const
  {
    auto tx = general_utils::minmax(m_finalStartPoint.x, m_finalEndPoint.x);
    auto ty = general_utils::minmax(m_finalStartPoint.y, m_finalEndPoint.y);
    auto startX = std::get<0>(tx);
    auto endX = std::get<1>(tx);
    auto startY = std::get<0>(ty);
    auto endY = std::get<1>(ty);

    startX -= m_size;
    startY -= m_size;
    endX += m_size;
    endY += m_size;
    return IntRect(startX,  startY, (endX - startX), (endY - startY));
  }

  std::string CircleTool::getDescription() const
  {
    return "Circle Tool";
  }
}
