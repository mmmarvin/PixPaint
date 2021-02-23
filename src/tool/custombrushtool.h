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
#ifndef CUSTOMBRUSHTOOL_H
#define CUSTOMBRUSHTOOL_H

#include "../image/pixeldata.h"
#include "cursortoolbase.h"

namespace pixpaint
{
  class Color;
  class MaskablePixelData;
  class ModifyablePixelData;
  class Point;

  class CustomBrushTool : public CursorToolBase
  {
    enum EMode
    {
      EM_SELECTION,
      EM_BRUSH
    };

  public:
    CustomBrushTool();

    int onKeyPress(EKey key,
                   const Color& color,
                   ControlState controlState,
                   ModifyablePixelData& previewLayer,
                   MaskablePixelData& currentLayer) override;
    bool onMousePress(const Point& currentPoint,
                      const Point& previousPoint,
                      const Point& globalPoint,
                      const Color& color,
                      ControlState controlState,
                      ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer) override;
    bool onMouseMove(const Point& currentPoint,
                     const Point& previousPoint,
                     const Point& globalPoint,
                     const Color& color,
                     ControlState controlState,
                     ModifyablePixelData& previewLayer,
                     MaskablePixelData& currentLayer) override;
    int onMouseRelease(const Point& currentPoint,
                       const Point& previousPoint,
                       const Point& globalPoint,
                       const Color& color,
                       ControlState controlState,
                       ModifyablePixelData& previewLayer,
                       MaskablePixelData& currentLayer) override;

    IntRect getDrawRect() const override;
    std::string getDescription() const override;

  private:
    PixelData m_brush;
//    std::string m_modeChoice;
    Point     m_min;
    Point     m_max;

    Point     m_startPoint;
    Point     m_endPoint;
    Point     m_finalStartPoint;
    Point     m_finalEndPoint;

    EMode     m_mode;
  };
}

#endif // CUSTOMBRUSHTOOL_H
