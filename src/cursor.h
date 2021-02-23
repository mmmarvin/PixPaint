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
#ifndef CURSOR_H
#define CURSOR_H

#include "image/pixeldata.h"

namespace pixpaint
{
  class Cursor
  {
  public:
    enum ECursorType
    {
      ECT_IMAGE,
      ECT_ARROW,
      ECT_CROSS,
      ECT_WAIT,
      ECT_IBEAM,
      ECT_RESIZE_VER,
      ECT_RESIZE_HOR,
      ECT_RESIZE_BDIAG,
      ECT_RESIZE_FDIAG,
      ECT_BUSY,
      ECT_OPEN_HAND,
      ECT_CLOSED_HAND,
      ECT_DRAG_COPY,
      ECT_DRAG_MOVE
    };

    Cursor();
    explicit Cursor(ECursorType cursorType);
    explicit Cursor(const PixelData& cursor, bool autoscale = true);

    bool isAutoScale() const noexcept;

    const PixelData& getCursor() const noexcept;
    ECursorType getCursorType() const noexcept;

  private:
    PixelData   m_cursor;
    ECursorType m_cursorType;
    bool        m_autoScale;
  };
}

#endif // CURSOR_H
