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

#include "../image/maskablepixeldata.h"
#include "../utility/general_utility.h"

namespace pixpaint
{
namespace paint_helpers
{
namespace paint_helpers_detail
{
  template<class ImageData>
  DoubleRect getDrawableRegion(const ImageData& imageData,
                               const Point& widgetDimension,
                               double pixelSize)
  {
    return DoubleRect(0,
                      0,
                      general_utils::min<double>((static_cast<double>(imageData.getWidth()) * pixelSize) + 1.0,
                                                 widgetDimension.x),
                      general_utils::min<double>((static_cast<double>(imageData.getHeight()) * pixelSize) + 1.0,
                                                 widgetDimension.y));
  }

  template<class ImageData>
  DoubleRect getSmallestDrawableRegion(const ImageData& imageData,
                                       const DoubleRect& option,
                                       const Point& widgetDimension,
                                       double pixelSize)
  {
    DoubleRect ret;

    auto drawableRegion = paint_helpers_detail::getDrawableRegion(imageData, widgetDimension, pixelSize);
    ret.x = general_utils::clamp<double>(option.x, 0, drawableRegion.width);
    ret.y = general_utils::clamp<double>(option.y, 0, drawableRegion.height);
    ret.width = general_utils::clamp<double>(option.width, 0, drawableRegion.width);
    ret.height = general_utils::clamp<double>(option.height, 0, drawableRegion.height);

    return ret;
  }
}
}
}
