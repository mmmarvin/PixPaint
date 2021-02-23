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
#ifndef FILLTOOL_H
#define FILLTOOL_H

#include <boost/functional/hash.hpp>
#include "../3rdparty/ska/unordered_map.hpp"
#include "../image/point.h"
#include "cursortoolbase.h"

namespace pixpaint
{
  class FillTool : public CursorToolBase
  {
  public:
    FillTool();

    bool onMousePress(const Point& currentPoint,
                      const Point& previousPoint,
                      const Point& globalPoint,
                      const Color& color,
                      ControlState controlState,
                      ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer) override;

    bool hasRightClick() const override;
    IntRect getDrawRect() const override;
    std::string getDescription() const override;

  private:
    Point   m_min;
    Point   m_max;
  };
}

#endif // FILLTOOL_H
