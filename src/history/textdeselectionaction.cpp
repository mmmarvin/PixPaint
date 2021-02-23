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
#include "textdeselectionaction.h"

namespace pixpaint
{
  TextDeselectionAction::TextDeselectionAction(const IntRect& rect,
                                               const MaskablePixelData& background,
                                               const std::string& text,
                                               const std::string& font,
                                               size_t size,
                                               bool antialiased) :
    TextSelectionAction("Text Deselect")
  {
    m_rect = rect;
    m_background = std::make_unique<MaskablePixelData>(background);
    m_text = text;
    m_font = font;
    m_size = size;
    m_antialiased = antialiased;
  }

  TextDeselectionAction::EActionResult TextDeselectionAction::undo()
  {
    return TextSelectionAction::redo();
  }

  TextDeselectionAction::EActionResult TextDeselectionAction::redo()
  {
    return TextSelectionAction::undo();
  }
}
