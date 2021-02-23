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
#include "textmanager.h"

#include <cctype>
#include <utility>
#include <vector>
#include "../embed/script_utility.h"
#include "../define.h"
#include "colormanager.h"
#include "textselectionmanager.h"

namespace pixpaint
{
  TextManager::TextManager() :
    m_text(""),
    m_cursorPosition(0),
    m_fontSize(11),
    m_antialiased(false)
  {
    auto font_family_list = script_utils::getFontList();
    if(!font_family_list.empty()) {
      m_fontFamily = *font_family_list.begin();
    }
  }

  void TextManager::onKeyPress(EKey key, bool shiftDown)
  {
    auto c = static_cast<char>(key);
    if(isalnum(c) || ispunct(c)) {
      if(key == Qt::Key_Control ||
         key == Qt::Key_Shift ||
         key == Qt::Key_CapsLock ||
         key == Qt::Key_Alt) {
        return;
      }

      if(shiftDown) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
      } else {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
      }

      m_text.insert(m_text.begin() + m_cursorPosition, c);
      ++m_cursorPosition;
      updateText();
    } else {
      switch(key) {
      case EKey::Key_Backspace:
        if(!m_text.empty()) {
          m_text.erase(m_text.begin() + (m_cursorPosition - 1));
          --m_cursorPosition;
          updateText();
        }
        break;
      case EKey::Key_Delete:
        if(!m_text.empty() && m_cursorPosition < m_text.size()) {
          m_text.erase(m_text.begin() + m_cursorPosition);
          updateText();
        }
        break;
      case EKey::Key_Space:
        m_text.insert(m_text.begin() + m_cursorPosition, ' ');
        ++m_cursorPosition;
        updateText();
        break;
      case EKey::Key_Home:
        m_cursorPosition = 0;
        this->updateText();
        break;
      case EKey::Key_End:
        m_cursorPosition = m_text.size();
        this->updateText();
        break;
      case EKey::Key_Left:
        if(m_cursorPosition > 0) {
          --m_cursorPosition;
        }
        this->updateText();
        break;
      case EKey::Key_Right:
        if(m_cursorPosition < m_text.size()) {
          ++m_cursorPosition;
        }
        this->updateText();
        break;
      case EKey::Key_Tab:
        m_text.insert(m_text.begin() + m_cursorPosition, '\t');
        ++m_cursorPosition;
        updateText();
        break;
      case EKey::Key_Enter:
      case EKey::Key_Return:
        m_text.insert(m_text.begin() + m_cursorPosition, '\n');
        ++m_cursorPosition;
        updateText();
        break;
      default:
        break;
      }
    }
  }

  bool TextManager::tryUpdateText(bool includeCursor)
  {
    if(getTextSelectionManager().selectionExists()) {
      updateText(includeCursor);
      return true;
    }

    return false;
  }

  void TextManager::addText(std::string text)
  {
    m_text.insert(m_text.begin() + m_cursorPosition, text.begin(), text.end());
    m_cursorPosition += text.size();
    tryUpdateText();
  }

  void TextManager::setText(std::string text)
  {
    m_text = std::move(text);
    m_cursorPosition = m_text.size();
    tryUpdateText();
  }

  void TextManager::clearText()
  {
    m_text.clear();
    m_cursorPosition = 0;
    tryUpdateText();
  }

  void TextManager::setFontFamily(std::string fontFamily)
  {
    m_fontFamily = std::move(fontFamily);
  }

  void TextManager::setFontSize(int fontSize)
  {
    m_fontSize = fontSize;
  }

  void TextManager::setAntiAliasing(bool antialiasing)
  {
    m_antialiased = antialiasing;
  }

  const std::string& TextManager::getText() const noexcept
  {
    return m_text;
  }

  const std::string& TextManager::getFontFamily() const noexcept
  {
    return m_fontFamily;
  }

  int TextManager::getFontSize() const noexcept
  {
    return m_fontSize;
  }

  bool TextManager::getAntialiasing() const noexcept
  {
    return m_antialiased;
  }

  void TextManager::updateText(bool includeCursor)
  {
    static const auto cursor_inserter = [] (bool includeCursor, std::string text, std::size_t cursorIndex) {
      if(includeCursor) {
        text.insert(text.begin() + cursorIndex, '|');
      }

      return text;
    };

    if(!m_fontFamily.empty()) {
      auto& text_selection_manager = getTextSelectionManager();
      const auto& color_manager = getColorManager();
      auto text_color = color_manager.getForegroundColor();
      auto background_color = color_manager.getBackgroundColor();
      auto text_selection_rect = text_selection_manager.getSelectionRect();

      text_selection_manager.getTextLayer().clear(color_manager.getBackgroundColor());
      auto text = cursor_inserter(includeCursor, m_text, m_cursorPosition);
      text_selection_manager.getLayerDrawer().drawText(text,
                                                       m_fontFamily,
                                                       m_fontSize,
                                                       m_antialiased,
                                                       Point(text_selection_rect.width, text_selection_rect.height),
                                                       text_color,
                                                       background_color);
    }
  }

  TextManager& getTextManager()
  {
    static TextManager instance;
    return instance;
  }
}
