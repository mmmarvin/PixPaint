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
    CursorToolBase("res/cursor/color_picker_cursor.png", false),
    m_selected(false)
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
    if(currentPoint.x >= 0 && currentPoint.y >= 0 &&
       currentPoint.x < currentLayer.getWidth() &&
       currentPoint.y < currentLayer.getHeight()) {
      getColorManager().setForegroundColor(currentLayer.getPixel(currentPoint.x, currentPoint.y));
      m_selected = true;
    }

    return false;
  }

  int ColorPickerTool::onMouseRelease(const Point&,
                                      const Point&,
                                      const Point&,
                                      const Color&,
                                      ControlState,
                                      ModifyablePixelData&,
                                      MaskablePixelData&)
  {
    if(m_selected) {
      getPaintToolManager().setToPreviousTool();
      m_selected = false;
    }

    return EChangeResult::ECCR_NONE;
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
