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
#include "cursortoolbase.h"

namespace pixpaint
{
  CursorToolBase::CursorToolBase(Qt::CursorShape cursorShape) :
    m_cursor(QCursor(cursorShape), false)
  {
  }

  CursorToolBase::CursorToolBase(const std::string& cursorLocation, bool autoScale) :
    m_cursor(QCursor(QPixmap(cursorLocation.c_str())), autoScale)
  {
  }

  CursorToolBase::CursorToolBase(const std::string& cursorLocation, int scaleX, int scaleY, bool autoScale) :
    m_cursor(QCursor(QPixmap(cursorLocation.c_str()).scaled(scaleX, scaleY)), autoScale)
  {
  }

  CursorToolBase::CursorToolBase(const QPixmap& cursor, bool autoScale) :
    m_cursor(QCursor(cursor), autoScale)
  {
  }

  CursorToolBase::CursorToolBase(const QPixmap& cursor, int scaleX, int scaleY, bool autoScale) :
    m_cursor(QCursor(cursor.scaled(scaleX, scaleY)), autoScale)
  {
  }

  optional<const Cursor&> CursorToolBase::getCursor() const
  {
    return m_cursor;
  }

  void CursorToolBase::setCursor(const Cursor& cursor)
  {
    m_cursor = cursor;
  }

  Cursor& CursorToolBase::getBaseCursor() noexcept
  {
    return m_cursor;
  }

  const Cursor& CursorToolBase::getBaseCursor() const noexcept
  {
    return m_cursor;
  }
}
