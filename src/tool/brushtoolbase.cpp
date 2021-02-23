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
#include "brushtoolbase.h"

#include "../image/color.h"
#include "../image/image.h"
#include "../image/maskablepixeldata.h"
#include "../manager/colormanager.h"
#include "../manager/drawermanager.h"
#include "../manager/imagemanager.h"
#include "../utility/general_utility.h"
#include "../utility/qt_utility.h"
#include "../debug_log.h"
#include "../define.h"

namespace pixpaint
{
namespace brushtoolbase_detail
{
  QPixmap generateBrushCursor(const Color& color)
  {
    QImage ret(1, 1, QImage::Format_RGBA8888);
    ret.setPixelColor(0, 0, qt_utils::convertToQTColor(color));

    return QPixmap::fromImage(ret);
  }
}
  static Drawer* currentDrawer = nullptr;

  BrushToolBase::BrushToolBase() :
    CursorToolBase(brushtoolbase_detail::generateBrushCursor()),
    m_size(1),
    m_antialiased(DEFAULT_TOOL_ANTIALIASING)
  {
  }

  bool BrushToolBase::onMousePress(const Point& currentPoint,
                                   const Point&,
                                   const Point&,
                                   const Color& color,
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

    currentDrawer = &getLayerDrawer();
    currentDrawer->drawLineBlend(currentPoint, currentPoint, color, DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });
    m_min = m_max = currentPoint;
    return true;
  }

  bool BrushToolBase::onMouseMove(const Point& currentPoint,
                                  const Point& previousPoint,
                                  const Point&,
                                  const Color& color,
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

      currentDrawer->drawLineBlend(previousPoint, currentPoint, color, DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });
    } else {
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

      currentDrawer->drawLineBlend(currentPoint, currentPoint, color, DrawParam{ m_antialiased, LineStyle{ m_size, 0, false, LineStyle::ELineStyle::SolidLine } });
    }

    auto tx = general_utils::minmax(currentPoint.x, previousPoint.x);
    auto ty = general_utils::minmax(currentPoint.y, previousPoint.y);
    auto minX = std::get<0>(tx);
    auto maxX = std::get<1>(tx);
    auto minY = std::get<0>(ty);
    auto maxY = std::get<1>(ty);

    minX -= m_size;
    maxX += m_size;
    minY -= m_size;
    maxY += m_size;

    m_min.x = minX - 1;
    m_min.y = minY - 1;
    m_max.x = maxX + 1;
    m_max.y = maxY + 1;
    return true;
  }

  bool BrushToolBase::onMouseRelease(const Point&,
                                     const Point&,
                                     const Point&,
                                     const Color&,
                                     ControlState,
                                     ModifyablePixelData&,
                                     MaskablePixelData&)
  {
    getImageManager().getImage().setModified(true);
    return true;
  }

  int BrushToolBase::onToolChange(ModifyablePixelData&, MaskablePixelData&)
  {
    auto foregroundColor = getColorManager().getForegroundColor();
    if(foregroundColor == Color::TRANSPARENT) {
      foregroundColor = Color::WHITE;
    }
    foregroundColor.a = 255;
    this->setCursor(Cursor(QCursor(brushtoolbase_detail::generateBrushCursor(foregroundColor).scaled(m_size, m_size))));

    return EChangeResult::ECCR_UPDATECURSOR;
  }

  int BrushToolBase::onColorChange(ModifyablePixelData&, MaskablePixelData&)
  {
    auto foregroundColor = getColorManager().getForegroundColor();
    if(foregroundColor == Color::TRANSPARENT) {
      foregroundColor = Color::WHITE;
    }
    foregroundColor.a = 255;
    this->setCursor(Cursor(QCursor(brushtoolbase_detail::generateBrushCursor(foregroundColor).scaled(m_size, m_size))));

    return EChangeResult::ECCR_UPDATECURSOR;
  }

  bool BrushToolBase::hasRightClick() const
  {
    return true;
  }

  IntRect BrushToolBase::getDrawRect() const
  {
    auto diff = m_max - m_min;
    return IntRect(m_min.x, m_min.y, general_utils::max(diff.x, 1), general_utils::max(diff.y, 1));
  }
}
