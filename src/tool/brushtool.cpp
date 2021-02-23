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
#include "brushtool.h"

#include "../manager/colormanager.h"

namespace pixpaint
{
  BrushTool::BrushTool()
  {
    this->addIntegerValueOption(&m_size, "Size", 1, 100);
    this->addFlagOption(&m_antialiased, "Anti-Aliased");
  }

  int BrushTool::onOptionChange(ModifyablePixelData&, MaskablePixelData&)
  {
    auto foregroundColor = getColorManager().getForegroundColor();
    if(foregroundColor == Color::TRANSPARENT) {
      foregroundColor = Color::WHITE;
    }
    foregroundColor.a = 255;
    this->setCursor(Cursor(QCursor(brushtoolbase_detail::generateBrushCursor(foregroundColor).scaled(m_size, m_size))));

    return EChangeResult::ECCR_UPDATECURSOR;
  }

  std::string BrushTool::getDescription() const
  {
    return "Brush Tool";
  }
}
