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
#ifndef RECTSELECTIONTOOL_H
#define RECTSELECTIONTOOL_H

#include "../image/point.h"
#include "../image/size.h"
#include "cursortoolbase.h"

namespace pixpaint
{
  class RectSelectionTool : public CursorToolBase
  {
  public:
    RectSelectionTool();

    void onPreMousePress() override;
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

    IntRect getDrawRect() const override;
    std::string getDescription() const override;

  private:
    Point m_startPoint;
    Point m_endPoint;
    Point m_finalStartPoint;
    Point m_finalEndPoint;

    Point m_min;
    Point m_max;
  };
}

#endif // RECTSELECTIONTOOL_H
