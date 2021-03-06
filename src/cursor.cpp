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
#include "cursor.h"

namespace pixpaint
{
  Cursor::Cursor() :
    m_cursor(0, 0),
    m_cursorType(ECT_ARROW),
    m_autoScale(false)
  {
  }

  Cursor::Cursor(ECursorType cursorType) :
    m_cursor(0, 0),
    m_cursorType(cursorType),
    m_autoScale(false)
  {
  }

  Cursor::Cursor(const PixelData& cursor, bool autoScale) :
    m_cursor(cursor),
    m_cursorType(ECursorType::ECT_IMAGE),
    m_autoScale(autoScale)
  {
  }

  bool Cursor::isAutoScale() const noexcept
  {
    return m_autoScale;
  }

  const PixelData& Cursor::getCursor() const noexcept
  {
    return m_cursor;
  }

  Cursor::ECursorType Cursor::getCursorType() const noexcept
  {
    return m_cursorType;
  }
}
