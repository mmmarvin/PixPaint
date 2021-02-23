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
#include "paint_helpers.h"

#include "../image/modifyablepixeldata.h"
#include "../tool/painttoolbase.h"
#include "../manager/imagemanager.h"
#include "../manager/painttoolmanager.h"
#include "../type.h"

namespace pixpaint
{
namespace paint_helpers
{
  bool paintStartEvent(position_t px,
                       position_t py,
                       const Color& color,
                       ControlState controlState,
                       Point& currentPoint,
                       Point& previousPoint,
                       const Point& globalPoint,
                       ModifyablePixelData& previewLayer,
                       MaskablePixelData& currentLayer)
  {
    previousPoint = Point(-1, -1);
    currentPoint = Point(px, py);

//    auto layer_mask_empty = !currentLayer.isMaskSet();
//    auto preview_mask_empty = !previewLayer.isMaskSet();
//    if(!layer_mask_empty && preview_mask_empty) {
//      previewLayer.setMask(currentLayer.getMask());
//    } else if(layer_mask_empty && !preview_mask_empty){
//      previewLayer.clearMask();
//    }

    return getPaintToolManager().getCurrentTool().onMousePress(currentPoint,
                                                               previousPoint,
                                                               globalPoint,
                                                               color,
                                                               controlState,
                                                               previewLayer,
                                                               currentLayer);
  }

  bool paintMoveEvent(position_t px,
                      position_t py,
                      const Color& color,
                      ControlState controlState,
                      Point& currentPoint,
                      Point& previousPoint,
                      const Point& globalPoint,
                      ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer)
  {
    previousPoint = currentPoint;
    currentPoint = Point(px, py);

    return getPaintToolManager().getCurrentTool().onMouseMove(currentPoint,
                                                              previousPoint,
                                                              globalPoint,
                                                              color,
                                                              controlState,
                                                              previewLayer,
                                                              currentLayer);
  }

  int paintEndEvent(position_t px,
                    position_t py,
                    const Color& color,
                    ControlState controlState,
                    Point& currentPoint,
                    Point& previousPoint,
                    const Point& globalPoint,
                    ModifyablePixelData& previewLayer,
                    MaskablePixelData& currentLayer)
  {
    previousPoint = currentPoint;
    currentPoint = Point(px, py);

    auto res = getPaintToolManager().getCurrentTool().onMouseRelease(currentPoint,
                                                                     previousPoint,
                                                                     globalPoint,
                                                                     color,
                                                                     controlState,
                                                                     previewLayer,
                                                                     currentLayer);

    previousPoint = currentPoint = Point(-1, -1);
    return res;
  }
}
}
