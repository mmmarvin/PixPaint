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
#include "rectselectiontool.h"

#include "../debug_log.h"
#include "../helper/selection_helpers.h"
#include "../image/color.h"
#include "../image/image.h"
#include "../image/modifyablepixeldata.h"
#include "../manager/drawermanager.h"
#include "../manager/dummyselectionmanager.h"
#include "../manager/selectionmanager.h"
#include "../utility/general_utility.h"

namespace pixpaint
{
  RectSelectionTool::RectSelectionTool() :
    CursorToolBase(Cursor::ECursorType::ECT_CROSS),
    m_min(-1, -1),
    m_max(1, 1)
  {
  }

  void RectSelectionTool::onPreMousePress()
  {
    selection_helpers::tryFinalizeAllSelections(true);
  }

  bool RectSelectionTool::onMousePress(const Point& currentPoint,
                                       const Point&,
                                       const Point&,
                                       const Color&,
                                       ControlState,
                                       ModifyablePixelData&,
                                       MaskablePixelData& currentLayer)
  {
    m_startPoint = m_endPoint = currentPoint;
    m_min  = Point(0, 0);
    m_max = Point(currentLayer.getWidth(), currentLayer.getHeight());

    return false;
  }

  bool RectSelectionTool::onMouseMove(const Point& currentPoint,
                                      const Point&,
                                      const Point&,
                                      const Color&,
                                      ControlState controlState,
                                      ModifyablePixelData&,
                                      MaskablePixelData&)
  {
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

    return true;
  }

  int RectSelectionTool::onMouseRelease(const Point&,
                                        const Point&,
                                        const Point&,
                                        const Color&,
                                        ControlState,
                                        ModifyablePixelData&,
                                        MaskablePixelData& currentLayer)
  {
    getDummySelectionManager().clear();

    auto& selection_manager = getSelectionManager();
    if(m_startPoint != m_endPoint &&
       m_finalStartPoint.x != m_finalEndPoint.x &&
       m_finalStartPoint.y != m_finalEndPoint.y &&
       !selection_manager.selectionExists() &&
       !selection_manager.layerExists()) {
      auto dim = m_finalEndPoint - m_finalStartPoint;
      selection_manager.setSelectionRect(m_finalStartPoint.x,
                                         m_finalStartPoint.y,
                                         dim.x + 1,
                                         dim.y + 1);
      selection_manager.setSelectionLayer(currentLayer.cut(m_finalStartPoint.x,
                                                           m_finalStartPoint.y,
                                                           dim.x + 1,
                                                           dim.y + 1));
    }

    return EChangeResult::ECCR_UPDATEIMAGE;
  }

  IntRect RectSelectionTool::getDrawRect() const
  {
    auto diff = m_max - m_min;
    return IntRect(m_min.x - 1,
                   m_min.y - 1,
                   general_utils::max(diff.x, 1) + 1,
                   general_utils::max(diff.y, 1) + 1);
  }

  std::string RectSelectionTool::getDescription() const
  {
    return "Rectangle Selection Tool";
  }
}
