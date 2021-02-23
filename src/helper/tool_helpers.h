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
#ifndef TOOL_HELPERS_H
#define TOOL_HELPERS_H

#include "../drawer/drawer.h"
#include "../tool/painttoolbase.h"
#include "../linestyle.h"

namespace pixpaint
{
  struct Color;
  struct Point;
  class MaskablePixelData;
  class ModifyablePixelData;

namespace tool_helpers
{
  void onToolChange(PaintToolHandlerBase& paintTool, int (PaintToolHandlerBase::*changeFunc)(ModifyablePixelData&, MaskablePixelData&));

  void setViewToolCursor(const Cursor& cursor);
  void updateViewToolCursor();

  void drawShape(Drawer* drawer,
                 void (Drawer::*shapeFunc)(const Point&, const Point&, const Color&, const Color&, const DrawParam&),
                 ControlState controlState,
                 Point& startPoint,
                 Point& endPoint,
                 Point& finalStartPoint,
                 Point& finalEndPoint,
                 Point& min,
                 Point& max,
                 bool fill,
                 const DrawParam& param);
}
}

#endif // TOOL_HELPERS_H
