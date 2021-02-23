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
#ifndef PAINT_HELPERS_H
#define PAINT_HELPERS_H

#include "../image/rect.h"
#include "../tool/painttoolbase.h"

namespace pixpaint
{
  struct Color;
  class ModifyablePixelData;
  class MaskablePixelData;
namespace paint_helpers
{
namespace paint_helpers_detail
{
  template<class ImageData>
  DoubleRect getDrawableRegion(const ImageData& imageData,
                               const Point& widgetDimension,
                               double pixelSize);
  template<class ImageData>
  DoubleRect getSmallestDrawableRegion(const MaskablePixelData& currentLayer,
                                       const DoubleRect& option,
                                       const Point& widgetDimension,
                                       double pixelSize);
}
  bool paintStartEvent(position_t px,
                       position_t py,
                       const Color& color,
                       ControlState controlState,
                       Point& currentPoint,
                       Point& previousPoint,
                       const Point& globalPoint,
                       ModifyablePixelData& previewLayer,
                       MaskablePixelData& currentLayer);
  bool paintMoveEvent(position_t px,
                      position_t py,
                      const Color& color,
                      ControlState controlState,
                      Point& currentPoint,
                      Point& previousPoint,
                      const Point& globalPoint,
                      ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer);
  bool paintEndEvent(position_t px,
                     position_t py,
                     const Color& color,
                     ControlState controlState,
                     Point& currentPoint,
                     Point& previousPoint,
                     const Point& globalPoint,
                     ModifyablePixelData& previewLayer,
                     MaskablePixelData& currentLayer);
}
}

#include "paint_helpers.inl"

#endif // PAINT_HELPERS_H
