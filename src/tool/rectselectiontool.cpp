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
#include "../utility/general_utility.h"
#include "../manager/selectionmanager.h"

namespace pixpaint
{
  RectSelectionTool::RectSelectionTool() :
    CursorToolBase(Qt::CrossCursor),
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
                                      ModifyablePixelData& previewLayer,
                                      MaskablePixelData&)
  {
    m_endPoint = currentPoint;

    auto startPoint = m_startPoint, endPoint = m_endPoint;
    bool negX = endPoint.x < startPoint.x, negY = endPoint.y < startPoint.y;
    if(negX) {
      std::swap(endPoint.x, startPoint.x);
    }
    if(negY) {
      std::swap(endPoint.y, startPoint.y);
    }

    Size dimension = endPoint - startPoint;
    if(controlState.shiftDown) {
      auto min = general_utils::min(dimension.width, dimension.height);
      dimension.width = dimension.height = min;
      if(negX) {
        startPoint.x = endPoint.x - dimension.width;
      } else {
        endPoint.x = startPoint.x + dimension.width;
      }
      if(negY) {
        startPoint.y = endPoint.y - dimension.height;
      } else {
        endPoint.y = startPoint.y + dimension.height;
      }
    }

    if(controlState.controlDown) {
      startPoint = m_startPoint - Point(dimension);
      endPoint = m_startPoint + Point(dimension);
      dimension *= 2;
    }

    if(dimension.width > 0 && dimension.height > 0)  {
      previewLayer.clear(Color::TRANSPARENT);
      getPreviewDrawer().drawRectBlend(startPoint,
                                       endPoint,
                                       Color::BLACK,
                                       Color::TRANSPARENT,
                                       DrawParam{ false, LineStyle{ 1, 0, false, LineStyle::DashedLine } });
      m_finalStartPoint = startPoint;
      m_finalEndPoint = endPoint;
    }

    return true;
  }

  bool RectSelectionTool::onMouseRelease(const Point&,
                                         const Point&,
                                         const Point&,
                                         const Color&,
                                         ControlState,
                                         ModifyablePixelData& previewLayer,
                                         MaskablePixelData& currentLayer)
  {
    previewLayer.clear(Color::TRANSPARENT);
    previewLayer.setModifiedFlag(false);

    auto& selectionManager = getSelectionManager();
    if(m_finalStartPoint.x != m_finalEndPoint.x &&
       m_finalStartPoint.y != m_finalEndPoint.y &&
       !selectionManager.selectionExists() &&
       !selectionManager.layerExists()) {
      auto dimension = m_finalEndPoint - m_finalStartPoint;
      selectionManager.setSelectionRect(m_finalStartPoint.x,
                                        m_finalStartPoint.y,
                                        dimension.x + 1,
                                        dimension.y + 1);
      selectionManager.setSelectionLayer(currentLayer.cut(m_finalStartPoint.x,
                                                          m_finalStartPoint.y,
                                                          dimension.x + 1,
                                                          dimension.y + 1));
    }

    return true;
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
