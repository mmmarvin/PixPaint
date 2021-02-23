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
#include "custombrushtool.h"

#include "../3rdparty/gengine/configuration.h"
#include "../image/image.h"
#include "../image/maskablepixeldata.h"
#include "../image/modifyablepixeldata.h"
#include "../image/pixeldata.h"
#include "../image/size.h"
#include "../manager/drawermanager.h"
#include "../manager/dummyselectionmanager.h"
#include "../manager/imagemanager.h"
#include "../utility/general_utility.h"
#include "../utility/geometry_utility.h"
#include "../debug_log.h"
#include "../define.h"
#include "brushtoolbase.h"

namespace pixpaint
{
  CustomBrushTool::CustomBrushTool() :
    CursorToolBase(Cursor::ECT_ARROW),
    m_brush(1, 1, Color::BLACK),
    m_mode(EM_BRUSH)
  {
//    this->addStringChoice(&m_modeChoice, "Mode", { "Brush Selection", "Brush" });
    this->setCursor(Cursor(m_brush));
  }

  int CustomBrushTool::onKeyPress(EKey key,
                                   const Color&,
                                   ControlState,
                                   ModifyablePixelData&,
                                   MaskablePixelData&)
  {
    if(key == EKey::Key_C) {
      m_mode = EMode::EM_SELECTION;
      this->setCursor(Cursor(Cursor::ECursorType::ECT_CROSS));

      return EChangeResult::ECCR_UPDATECURSOR;
    }
    if(m_mode == EM_SELECTION && key == EKey::Key_Escape) {
      m_mode = EMode::EM_BRUSH;
    }

    return EChangeResult::ECCR_NONE;
  }

  bool CustomBrushTool::onMousePress(const Point& currentPoint,
                                     const Point&,
                                     const Point&,
                                     const Color&,
                                     ControlState,
                                     ModifyablePixelData&,
                                     MaskablePixelData& currentLayer)
  {
    if(m_mode == EM_BRUSH) {
      auto w = m_brush.getWidth();
      auto h = m_brush.getHeight();
      auto hw = w / 2;
      auto hh = h / 2;

      auto start = currentPoint - Point(w, h);
      auto end = currentPoint + Point(w, h);
      getLayerDrawer().getDrawerCommander()->addCommand(start.x, start.y, end.x - start.x, end.y - start.y);
      currentLayer.combine(m_brush, currentPoint.x - hw, currentPoint.y - hh);

      auto sp = Point(w, h);
      m_min = currentPoint - sp;
      m_max = currentPoint + sp;
    } else if(m_mode == EM_SELECTION) {
      m_startPoint = m_endPoint = currentPoint;
      m_min  = Point(0, 0);
      m_max = Point(currentLayer.getWidth(), currentLayer.getHeight());
    }

    return true;
  }

  bool CustomBrushTool::onMouseMove(const Point& currentPoint,
                                    const Point& previousPoint,
                                    const Point&,
                                    const Color&,
                                    ControlState controlState,
                                    ModifyablePixelData&,
                                    MaskablePixelData& currentLayer)
  {
    if(m_mode == EM_BRUSH) {
      auto tx = general_utils::minmax(currentPoint.x, previousPoint.x);
      auto ty = general_utils::minmax(currentPoint.y, previousPoint.y);
      auto start_x = std::get<0>(tx);
      auto end_x = std::get<1>(tx);
      auto start_y = std::get<0>(ty);
      auto end_y = std::get<1>(ty);

      auto bw = m_brush.getWidth();
      auto bh = m_brush.getHeight();

      start_x -= bw;
      start_y -= bh;
      end_x += bw;
      end_y += bh;

      getLayerDrawer().getDrawerCommander()->addCommand(start_x, start_y, end_x - start_x, end_y - start_y);
      geometry_utils::drawLine(m_brush, currentLayer, currentPoint, previousPoint);

      m_min.x = start_x - 1;
      m_min.y = start_y - 1;
      m_max.x = end_x + 1;
      m_max.y = end_y + 1;
    } else if(m_mode == EM_SELECTION) {
      m_endPoint = currentPoint;

      auto start_point = m_startPoint, end_point = m_endPoint;
      bool neg_x = end_point.x < start_point.x, neg_y = end_point.y < start_point.y;
      if(neg_x) {
        std::swap(end_point.x, start_point.x);
      }
      if(neg_y) {
        std::swap(end_point.y, start_point.y);
      }

      Size dim = end_point - start_point;
      if(controlState.shiftDown) {
        auto min = general_utils::min(dim.width, dim.height);
        dim.width = dim.height = min;
        if(neg_x) {
          start_point.x = end_point.x - dim.width;
        } else {
          end_point.x = start_point.x + dim.width;
        }
        if(neg_y) {
          start_point.y = end_point.y - dim.height;
        } else {
          end_point.y = start_point.y + dim.height;
        }
      }

      if(controlState.controlDown) {
        start_point = m_startPoint - Point(dim);
        end_point = m_startPoint + Point(dim);
        dim *= 2;
      }

      if(dim.width > 0 && dim.height > 0)  {
        auto dim = end_point - start_point;
        getDummySelectionManager().setSelectionRect(IntRect(start_point.x, start_point.y, dim.x + 1, dim.y + 1));

        m_finalStartPoint = start_point;
        m_finalEndPoint = end_point;
      }
    }

    return true;
  }

  int CustomBrushTool::onMouseRelease(const Point&,
                                      const Point&,
                                      const Point&,
                                      const Color&,
                                      ControlState,
                                      ModifyablePixelData&,
                                      MaskablePixelData& currentLayer)
  {
    if(m_mode == EM_BRUSH) {
      getImageManager().getImage().setModified(true);
    } else if(m_mode == EM_SELECTION) {
      getDummySelectionManager().clear();
      if(m_finalStartPoint.x != m_finalEndPoint.x &&
         m_finalStartPoint.y != m_finalEndPoint.y) {
        auto dim = m_finalEndPoint - m_finalStartPoint;

        m_brush = currentLayer.copy(m_finalStartPoint.x,
                                    m_finalStartPoint.y,
                                    dim.x + 1,
                                    dim.y + 1);
        this->setCursor(Cursor(m_brush));

        m_mode = EM_BRUSH;
        return EChangeResult::ECCR_UPDATECURSOR;
      }
    }

    return EChangeResult::ECCR_NONE;
  }

  IntRect CustomBrushTool::getDrawRect() const
  {
    auto diff = m_max - m_min;
    return IntRect(m_min.x, m_min.y, general_utils::max(diff.x, 1), general_utils::max(diff.y, 1));
  }

  std::string CustomBrushTool::getDescription() const
  {
    return "Custom Brush Tool";
  }
}
