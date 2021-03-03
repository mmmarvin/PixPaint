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
#include "../utility/geometry_utility.h"
#include "../utility/qt_utility.h"
#include "../debug_log.h"
#include "../define.h"

namespace pixpaint
{
  BrushToolBase::BrushToolBase() :
    CursorToolBase(PixelData(1, 1, Color::BLACK)),
    m_brush(1, 1, Color::BLACK),
    m_size(1)
  {
  }

  bool BrushToolBase::onMousePress(const Point& currentPoint,
                                   const Point&,
                                   const Point&,
                                   const Color& color,
                                   ControlState,
                                   ModifyablePixelData&,
                                   MaskablePixelData& currentLayer)
  {
    auto tx = general_utils::minmax(currentPoint.x, currentPoint.x);
    auto ty = general_utils::minmax(currentPoint.y, currentPoint.y);
    auto start_x = std::get<0>(tx);
    auto end_x = std::get<1>(tx);
    auto start_y = std::get<0>(ty);
    auto end_y = std::get<1>(ty);

    start_x -= m_size;
    start_y -= m_size;
    end_x += m_size;
    end_y += m_size;

    m_brush = PixelData(m_size, m_size, color);

    auto hs = m_size / 2;
    getLayerDrawer().getDrawerCommander()->addCommand(start_x,
                                                      start_y,
                                                      end_x - start_x,
                                                      end_y - start_y);
    currentLayer.combine(m_brush, currentPoint.x - hs, currentPoint.y - hs);

    auto sp = Point(m_size, m_size);
    m_min = currentPoint - sp;
    m_max = currentPoint + sp;
    return true;
  }

  bool BrushToolBase::onMouseMove(const Point& currentPoint,
                                  const Point& previousPoint,
                                  const Point&,
                                  const Color&,
                                  ControlState,
                                  ModifyablePixelData&,
                                  MaskablePixelData& currentLayer)
  {
    if(previousPoint.x != -1 &&
       previousPoint.y != -1) {
      auto tx = general_utils::minmax(previousPoint.x, currentPoint.x);
      auto ty = general_utils::minmax(previousPoint.y, currentPoint.y);
      auto start_x = std::get<0>(tx);
      auto end_x = std::get<1>(tx);
      auto start_y = std::get<0>(ty);
      auto end_y = std::get<1>(ty);

      start_x -= m_size;
      start_y -= m_size;
      end_x += m_size;
      end_y += m_size;

      getLayerDrawer().getDrawerCommander()->addCommand(start_x,
                                                        start_y,
                                                        end_x - start_x,
                                                        end_y - start_y);
      geometry_utils::drawLine(m_brush, currentLayer, currentPoint, previousPoint);
    } else {
      auto tx = general_utils::minmax(currentPoint.x, currentPoint.x);
      auto ty = general_utils::minmax(currentPoint.y, currentPoint.y);
      auto start_x = std::get<0>(tx);
      auto end_x = std::get<1>(tx);
      auto start_y = std::get<0>(ty);
      auto end_y = std::get<1>(ty);

      start_x -= m_size;
      start_y -= m_size;
      end_x += m_size;
      end_y += m_size;

      getLayerDrawer().getDrawerCommander()->addCommand(start_x,
                                                        start_y,
                                                        end_x - start_x,
                                                        end_y - start_y);
      geometry_utils::drawLine(m_brush, currentLayer, currentPoint, previousPoint);
    }

    auto tx = general_utils::minmax(currentPoint.x, previousPoint.x);
    auto ty = general_utils::minmax(currentPoint.y, previousPoint.y);
    auto min_x = std::get<0>(tx);
    auto max_x = std::get<1>(tx);
    auto min_y = std::get<0>(ty);
    auto max_y = std::get<1>(ty);

    min_x -= m_size;
    max_x += m_size;
    min_y -= m_size;
    max_y += m_size;

    m_min.x = min_x - 1;
    m_min.y = min_y - 1;
    m_max.x = max_x + 1;
    m_max.y = max_y + 1;
    return true;
  }

  int BrushToolBase::onMouseRelease(const Point&,
                                    const Point&,
                                    const Point&,
                                    const Color&,
                                    ControlState,
                                    ModifyablePixelData&,
                                    MaskablePixelData&)
  {
    getImageManager().getImage().setModified(true);
    return EChangeResult::ECCR_NONE;
  }

  int BrushToolBase::onToolChange(ModifyablePixelData&, MaskablePixelData&)
  {
    auto foreground_color = getColorManager().getForegroundColor();
    if(foreground_color == Color::TRANSPARENT) {
      foreground_color = Color::WHITE;
    }
    foreground_color.a = 255;

    m_brush = PixelData(m_size, m_size, foreground_color);
    this->setCursor(Cursor(m_brush));

    return EChangeResult::ECCR_UPDATECURSOR;
  }

  int BrushToolBase::onColorChange(ModifyablePixelData&, MaskablePixelData&)
  {
    auto foreground_color = getColorManager().getForegroundColor();
    if(foreground_color == Color::TRANSPARENT) {
      foreground_color = Color::WHITE;
    }
    foreground_color.a = 255;

    m_brush = PixelData(m_size, m_size, foreground_color);
    this->setCursor(Cursor(m_brush));

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
