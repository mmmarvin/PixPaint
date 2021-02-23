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
#include "erasertool.h"

#include "../image/color.h"
#include "../image/image.h"
#include "../image/maskablepixeldata.h"
#include "../utility/general_utility.h"
#include "../manager/colormanager.h"
#include "../manager/drawermanager.h"
#include "../manager/imagemanager.h"
#include "../utility/qt_utility.h"

namespace pixpaint
{
namespace
{
  QPixmap getEraserCursor()
  {
    QImage ret(1, 1, QImage::Format_RGBA8888);
    ret.setPixelColor(0, 0, qt_utils::convertToQTColor(Color::WHITE));

    return QPixmap::fromImage(ret);
  }
}
  EraserTool::EraserTool() :
    CursorToolBase(getEraserCursor()),
    m_size(1),
    m_antialiasing(false)
  {
    this->addIntegerValueOption(&m_size, "Size", 1, 100);
    this->addFlagOption(&m_antialiasing, "Anti-Aliased");
  }

  bool EraserTool::onMousePress(const Point& currentPoint,
                                const Point&,
                                const Point&,
                                const Color&,
                                ControlState,
                                ModifyablePixelData&,
                                MaskablePixelData&)
  {
    auto tx = general_utils::minmax(currentPoint.x, currentPoint.x);
    auto ty = general_utils::minmax(currentPoint.y, currentPoint.y);
    auto startX = std::get<0>(tx);
    auto endX = std::get<1>(tx);
    auto startY = std::get<0>(ty);
    auto endY = std::get<1>(ty);

    startX -= m_size;
    startY -= m_size;
    endX += m_size;
    endY += m_size;

    getLayerDrawer().drawLine(currentPoint,
                              currentPoint,
                              Color::TRANSPARENT,
                              DrawParam{ m_antialiasing, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });
    m_currentMin = m_currentMax = currentPoint;

    return true;
  }

  bool EraserTool::onMouseMove(const Point& currentPoint,
                               const Point& previousPoint,
                               const Point&,
                               const Color&,
                               ControlState,
                               ModifyablePixelData&,
                               MaskablePixelData&)
  {
    if(previousPoint.x != -1 &&
       previousPoint.y != -1) {
      auto tx = general_utils::minmax(previousPoint.x, currentPoint.x);
      auto ty = general_utils::minmax(previousPoint.y, currentPoint.y);
      auto startX = std::get<0>(tx);
      auto endX = std::get<1>(tx);
      auto startY = std::get<0>(ty);
      auto endY = std::get<1>(ty);

      startX -= m_size;
      startY -= m_size;
      endX += m_size;
      endY += m_size;

      getLayerDrawer().drawLine(currentPoint,
                                previousPoint,
                                Color::TRANSPARENT,
                                DrawParam{ m_antialiasing, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });
    } else {
      auto tx = general_utils::minmax(previousPoint.x, currentPoint.x);
      auto ty = general_utils::minmax(previousPoint.y, currentPoint.y);
      auto startX = std::get<0>(tx);
      auto endX = std::get<1>(tx);
      auto startY = std::get<0>(ty);
      auto endY = std::get<1>(ty);

      startX -= m_size;
      startY -= m_size;
      endX += m_size;
      endY += m_size;

      getLayerDrawer().drawLine(currentPoint,
                                currentPoint,
                                Color::TRANSPARENT,
                                DrawParam{ m_antialiasing, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });
    }

    auto tx = general_utils::minmax(currentPoint.x, previousPoint.x);
    auto ty = general_utils::minmax(currentPoint.y, previousPoint.y);
    auto minX = std::get<0>(tx);
    auto maxX = std::get<1>(tx);
    auto minY = std::get<0>(ty);
    auto maxY = std::get<1>(ty);

    m_currentMin.x = minX - 1;
    m_currentMin.y = minY - 1;
    m_currentMax.x = maxX + 1;
    m_currentMax.y = maxY + 1;
    return true;
  }

  bool EraserTool::onMouseRelease(const Point&,
                                  const Point&,
                                  const Point&,
                                  const Color&,
                                  ControlState,
                                  ModifyablePixelData&,
                                  MaskablePixelData&)
  {
    getImageManager().getImage().setModified(true);
    return false;
  }

  int EraserTool::onToolChange(ModifyablePixelData&, MaskablePixelData&)
  {
    this->setCursor(Cursor(QCursor(getEraserCursor().scaled(m_size, m_size))));

    return EChangeResult::ECCR_UPDATECURSOR;
  }

  int EraserTool::onOptionChange(ModifyablePixelData&, MaskablePixelData&)
  {
    this->setCursor(Cursor(QCursor(getEraserCursor().scaled(m_size, m_size))));

    return EChangeResult::ECCR_UPDATECURSOR;
  }

  IntRect EraserTool::getDrawRect() const
  {
      auto min = m_currentMin;
      auto max = m_currentMax;

      min.x -= m_size;
      min.y -= m_size;
      max.x += m_size;
      max.y += m_size;

      auto diff = max - min;
      return IntRect(min.x,
                     min.y,
                     general_utils::max(diff.x, 1) + 1,
                     general_utils::max(diff.y, 1) + 1);;
  }

  std::string EraserTool::getDescription() const
  {
    return "Eraser Tool";
  }
}
