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
#include "textselectiontool.h"

#include "../embed/script_utility.h"
#include "../helper/selection_helpers.h"
#include "../manager/colormanager.h"
#include "../manager/textmanager.h"
#include "../manager/textselectionmanager.h"
#include "../utility/qt_utility.h"
#include "../debug_log.h"
#include "../define.h"

namespace pixpaint
{
  TextSelectionTool::TextSelectionTool() :
    CursorToolBase(Qt::CrossCursor),
    m_fontFamily(""),
    m_fontSize(11),
    m_antialiased(DEFAULT_TEXT_ANTIALIASING)
  {    
    auto font_family_list = script_utils::getFontList();
    if(!font_family_list.empty()) {
      m_fontFamily = *font_family_list.begin();
    }

    this->addStringChoice(&m_fontFamily, "Font Family", font_family_list);
    this->addIntegerValueOption(&m_fontSize, "Font Size", 1, 100);
    this->addFlagOption(&m_antialiased, "Anti-Aliased");
  }

  void TextSelectionTool::onPreMousePress()
  {
    selection_helpers::tryFinalizeAllSelections(true);
  }

  bool TextSelectionTool::onMousePress(const Point& currentPoint,
                                       const Point&,
                                       const Point&,
                                       const Color&,
                                       ControlState,
                                       ModifyablePixelData&,
                                       MaskablePixelData&)
  {
    auto& text_manager = getTextManager();
    auto& text_selection_manager = getTextSelectionManager();
    if(!text_selection_manager.selectionExists()) {
      text_manager.clearText();
      text_selection_manager.setSelectionRect(currentPoint.x,
                                            currentPoint.y,
                                            100,
                                            50);
      text_selection_manager.getTextLayer().clear(getColorManager().getBackgroundColor());
      text_manager.tryUpdateText();
      return true;
    }

    return false;
  }

  int TextSelectionTool::onOptionChange(ModifyablePixelData&,
                                         MaskablePixelData&)
  {
    auto& text_manager = getTextManager();
    text_manager.setFontFamily(m_fontFamily);
    text_manager.setFontSize(m_fontSize);
    text_manager.setAntiAliasing(m_antialiased);
    text_manager.tryUpdateText();

    return EChangeResult::ECCR_UPDATEIMAGE;
  }

  IntRect TextSelectionTool::getDrawRect() const
  {
    return getTextSelectionManager().getSelectionRect();
  }

  std::string TextSelectionTool::getDescription() const
  {
    return "Text Tool";
  }
}
