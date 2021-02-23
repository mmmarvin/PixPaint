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
#ifndef TEXTSELECTIONTOOL_H
#define TEXTSELECTIONTOOL_H

#include <string>
#include "../image/point.h"
#include "cursortoolbase.h"

namespace pixpaint
{
  class TextSelectionTool : public CursorToolBase
  {
  public:
    TextSelectionTool();

    void onPreMousePress() override;
    bool onMousePress(const Point& currentPoint,
                      const Point& previousPoint,
                      const Point& globalPoint,
                      const Color& color,
                      ControlState controlState,
                      ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer) override;

    int onOptionChange(ModifyablePixelData& previewLayer,
                       MaskablePixelData& currentLayer) override;

    IntRect getDrawRect() const override;
    std::string getDescription() const override;

  private:
    std::string m_fontFamily;
    int         m_fontSize;
    bool        m_antialiased;
  };
}

#endif // TEXTSELECTIONTOOL_H
