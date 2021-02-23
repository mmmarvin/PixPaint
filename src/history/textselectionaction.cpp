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
#include "textselectionaction.h"

#include "../image/image.h"
#include "../manager/imagemanager.h"
#include "../manager/textmanager.h"
#include "../manager/textselectionmanager.h"

namespace pixpaint
{
  TextSelectionAction::TextSelectionAction(std::string name) :
    HistoryAction(std::move(name))
  {
  }

  TextSelectionAction::EActionResult TextSelectionAction::undo()
  {
    auto& text_selection_manager = getTextSelectionManager();
    auto& text_manager = getTextManager();
    auto& image_manager = getImageManager();

    m_rect = text_selection_manager.getSelectionRect();
    m_text = text_manager.getText();
    m_font = text_manager.getFontFamily();
    m_size = text_manager.getFontSize();
    m_antialiased = text_manager.getAntialiasing();
    m_background = std::make_unique<MaskablePixelData>(image_manager.getImage().getCurrentLayer().copy(m_rect.x,
                                                                                                       m_rect.y,
                                                                                                       m_rect.width,
                                                                                                       m_rect.height));

    text_selection_manager.clear();
    text_manager.clearText();

    return EActionResult::EAR_REFRESH_IMAGE;
  }

  TextSelectionAction::EActionResult TextSelectionAction::redo()
  {
    auto& text_selection_manager = getTextSelectionManager();
    auto& text_manager = getTextManager();
    auto& image_manager = getImageManager();

    text_selection_manager.setSelectionRect(m_rect, true, false);
    text_manager.setFontFamily(m_font);
    text_manager.setFontSize(m_size);
    text_manager.setAntiAliasing(m_antialiased);
    text_manager.setText(m_text);
    image_manager.getImage().getCurrentLayer().combine(*m_background.get(),
                                                       m_rect.x,
                                                       m_rect.y,
                                                       true);
    m_background.reset(nullptr);

    return EActionResult::EAR_REFRESH_IMAGE;
  }
}
