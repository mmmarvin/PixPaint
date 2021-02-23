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
#ifndef CURSORTOOLBASE_H
#define CURSORTOOLBASE_H

#include "painttoolbase.h"

namespace pixpaint
{
  class CursorToolBase : public PaintToolBase
  {
  public:
    explicit CursorToolBase(Cursor::ECursorType cursorShape);
    explicit CursorToolBase(const std::string& cursorLocation, bool autoScale = true);
    CursorToolBase(const std::string& cursorLocation, int scaleX, int scaleY, bool autoScale = true);
    explicit CursorToolBase(const PixelData& cursor, bool autoScale = true);

    optional<const Cursor&> getCursor() const override;

  protected:
    void setCursor(const Cursor& cursor);

    Cursor& getBaseCursor() noexcept;
    const Cursor& getBaseCursor() const noexcept;

  private:
    Cursor m_cursor;
  };
}

#endif // CURSORTOOLBASE_H
