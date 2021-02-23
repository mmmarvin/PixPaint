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
#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <cstddef>
#include <string>
#include "../keys.h"

namespace pixpaint
{
  class TextManager
  {
    TextManager();

  public:
    void onKeyPress(EKey key, bool shiftDown);
    bool tryUpdateText(bool includeCursor = true);

    void addText(std::string text);
    void setText(std::string text);
    void clearText();

    void setFontFamily(std::string fontFamily);
    void setFontSize(int fontSize);
    void setAntiAliasing(bool antialiasing);

    const std::string& getText() const noexcept;
    const std::string& getFontFamily() const noexcept;
    int getFontSize() const noexcept;
    bool getAntialiasing() const noexcept;

  private:
    friend TextManager& getTextManager();

    void updateText(bool includeCursor = true);

    std::string m_text;
    std::string m_fontFamily;
    std::size_t m_cursorPosition;
    int         m_fontSize;
    bool        m_antialiased;
  };

  TextManager& getTextManager();
}

#endif // TEXTMANAGER_H
