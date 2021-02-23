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
#ifndef BRUSHTOOLBASE_H
#define BRUSHTOOLBASE_H

#include "../image/color.h"
#include "../image/point.h"
#include "cursortoolbase.h"

namespace pixpaint
{
namespace brushtoolbase_detail
{
  QPixmap generateBrushCursor(const Color& color = Color::BLACK);
}
  class BrushToolBase : public CursorToolBase
  {
  public:
    BrushToolBase();

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
    bool onMouseRelease(const Point& currentPoint,
                        const Point& previousPoint,
                        const Point& globalPoint,
                        const Color& color,
                        ControlState controlState,
                        ModifyablePixelData& previewLayer,
                        MaskablePixelData& currentLayer) override;

    int onToolChange(ModifyablePixelData& previewLayer, MaskablePixelData& currentLayer) override;
    int onColorChange(ModifyablePixelData& previewLayer, MaskablePixelData& currentLayer) override;

    bool hasRightClick() const override;
    IntRect getDrawRect() const override;

  protected:
    Point m_min;
    Point m_max;
    int   m_size;
    bool  m_antialiased;
  };
}

#endif // BRUSHTOOLBASE_H
