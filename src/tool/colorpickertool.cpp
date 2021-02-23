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
#include "colorpickertool.h"

#include "../image/image.h"
#include "../manager/colormanager.h"
#include "../manager/painttoolmanager.h"

namespace pixpaint
{
  ColorPickerTool::ColorPickerTool() :
    CursorToolBase("res/cursor/color_picker_cursor.png", false)
  {
  }

  bool ColorPickerTool::onMousePress(const Point& currentPoint,
                                     const Point&,
                                     const Point&,
                                     const Color&,
                                     ControlState,
                                     ModifyablePixelData&,
                                     MaskablePixelData& currentLayer)
  {
    getColorManager().setForegroundColor(currentLayer.getPixel(currentPoint.x, currentPoint.y));
    return true;
  }

  bool ColorPickerTool::onMouseRelease(const Point&,
                                       const Point&,
                                       const Point&,
                                       const Color&,
                                       ControlState,
                                       ModifyablePixelData&,
                                       MaskablePixelData&)
  {
    getPaintToolManager().setToPreviousTool();
    return true;
  }

  bool ColorPickerTool::hasRightClick() const
  {
    return true;
  }

  IntRect ColorPickerTool::getDrawRect() const
  {
    return IntRect(-1, -1, 0, 0);
  }

  std::string ColorPickerTool::getDescription() const
  {
    return "Color Picker Tool";
  }
}
