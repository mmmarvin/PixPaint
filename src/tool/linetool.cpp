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
#include "linetool.h"

#include "../helper/tool_helpers.h"
#include "../image/color.h"
#include "../image/image.h"
#include "../image/modifyablepixeldata.h"
#include "../manager/drawermanager.h"
#include "../manager/imagemanager.h"
#include "../utility/float_utility.h"
#include "../utility/general_utility.h"
#include "../debug_log.h"
#include "../define.h"

namespace pixpaint
{
  static Drawer* currentDrawer = nullptr;

  LineTool::LineTool() :
    CursorToolBase(Qt::CrossCursor),
    m_size(1),
    m_antialiased(DEFAULT_TOOL_ANTIALIASING)
  {
    this->addIntegerValueOption(&m_size, "Size", 1, 100);
    this->addFlagOption(&m_antialiased, "Anti-Aliased");
  }

  bool LineTool::onMousePress(const Point& currentPoint,
                              const Point&,
                              const Point&,
                              const Color& color,
                              ControlState,
                              ModifyablePixelData& previewLayer,
                              MaskablePixelData&)
  {
    m_startPoint = m_endPoint = currentPoint;
    m_lineStartPoint = m_startPoint;
    m_lineEndPoint = m_endPoint;
    m_min = m_max = currentPoint;

    currentDrawer = &getPreviewDrawer();

    previewLayer.clear(Color::TRANSPARENT);
    currentDrawer->drawLineBlend(m_startPoint,
                                 m_endPoint,
                                 color,
                                 DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });

    return true;
  }

  bool LineTool::onMouseMove(const Point& currentPoint,
                             const Point&,
                             const Point&,
                             const Color& color,
                             ControlState controlState,
                             ModifyablePixelData& previewLayer,
                             MaskablePixelData&)
  {
    m_endPoint = currentPoint;

    previewLayer.clear(Color::TRANSPARENT);
    if(controlState.shiftDown) {
      auto x_diff = (static_cast<float>(m_endPoint.x) - static_cast<float>(m_startPoint.x));
      auto y_diff = (static_cast<float>(m_endPoint.y) - static_cast<float>(m_startPoint.y));
      if(!float_utils::isZero(x_diff)) {
        auto slope = y_diff / x_diff;
        if(x_diff > 0.f) {
          if(slope <= -2.f) {
            m_endPoint.x = m_startPoint.x;
          } else if(slope > -2.f && slope <= -0.5f) {
            m_endPoint.y = m_startPoint.y - general_utils::fastAbs(x_diff);
          } else if(slope > -0.5f && slope <= 0.5f) {
            m_endPoint.y = m_startPoint.y;
          } else if(slope > 0.5f && slope <= 2.f){
            m_endPoint.y = m_startPoint.y + general_utils::fastAbs(x_diff);
          } else if(slope > 2.f){
            m_endPoint.x = m_startPoint.x;
          }
        } else {
          if(slope >= 2.f) {
            m_endPoint.x = m_startPoint.x;
          } else if(slope < 2.f && slope >= 0.5f) {
            m_endPoint.y = m_startPoint.y - general_utils::fastAbs(x_diff);
          } else if(slope < 0.5f && slope >= -0.5f) {
            m_endPoint.y = m_startPoint.y;
          } else if(slope < -0.5f && slope >= -2.f) {
            m_endPoint.y = m_startPoint.y + general_utils::fastAbs(x_diff);
          } else if(slope < -2.f) {
            m_endPoint.x = m_startPoint.x;
          }
        }
      }
    }

    m_lineStartPoint = m_startPoint, m_lineEndPoint = m_endPoint;
    if(m_lineStartPoint.x < m_lineEndPoint.x ||
       m_lineStartPoint.y < m_lineEndPoint.y) {
      std::swap(m_lineStartPoint, m_lineEndPoint);
    }

    if(controlState.controlDown) {
      auto len = length(m_lineEndPoint, m_lineStartPoint);
      auto diff = m_lineEndPoint - m_lineStartPoint;
      auto dirX = float(diff.x);
      auto dirY = float(diff.y);
      auto m = magnitude(diff);
      if(m > 0.f) {
        dirX /= m;
        dirY /= m;
      }

      m_lineStartPoint = m_startPoint - Point(dirX * len, dirY * len);
      m_lineEndPoint = m_startPoint + Point(dirX * len, dirY * len);
    }

    currentDrawer->drawLineBlend(m_lineStartPoint,
                                 m_lineEndPoint,
                                 color,
                                 DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });

    m_min.x = general_utils::minelement(std::initializer_list<position_t>{ m_lineStartPoint.x, m_lineEndPoint.x, m_min.x });
    m_min.y = general_utils::minelement(std::initializer_list<position_t>{ m_lineStartPoint.y, m_lineEndPoint.y, m_min.y });
    m_max.x = general_utils::maxelement(std::initializer_list<position_t>{ m_lineStartPoint.x, m_lineEndPoint.x, m_max.x });
    m_max.y = general_utils::maxelement(std::initializer_list<position_t>{ m_lineStartPoint.y, m_lineEndPoint.y, m_max.y });
    return true;
  }

  bool LineTool::onMouseRelease(const Point&,
                                const Point&,
                                const Point&,
                                const Color& color,
                                ControlState,
                                ModifyablePixelData& previewLayer,
                                MaskablePixelData&)
  {
    previewLayer.clear(Color::TRANSPARENT);
    getLayerDrawer().drawLineBlend(m_lineStartPoint,
                                   m_lineEndPoint,
                                   color,
                                   DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });

    getImageManager().getImage().setModified(true);
    return true;
  }

  bool LineTool::hasRightClick() const
  {
    return true;
  }

  IntRect LineTool::getDrawRect() const
  {
    auto min = m_min;
    auto max = m_max;

    min.x -= m_size;
    min.y -= m_size;
    max.x += m_size;
    max.y += m_size;

    auto diff = max - min;
    return IntRect(min.x,
                   min.y,
                   general_utils::max(diff.x, 1) + 1,
                   general_utils::max(diff.y, 1) + 1);
  }

  optional<IntRect> LineTool::getHistoryRect() const
  {
    auto tx = general_utils::minmax(m_startPoint.x, m_endPoint.x);
    auto ty = general_utils::minmax(m_startPoint.y, m_endPoint.y);
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

  std::string LineTool::getDescription() const
  {
    return "Line Tool";
  }
}
