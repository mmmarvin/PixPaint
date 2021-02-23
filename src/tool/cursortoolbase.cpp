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

#include "../3rdparty/stbi/stb_image.h"
#include "../filetype/stbiimageholder.h"
#include "../image/image.h"
#include "../debug_log.h"

namespace pixpaint
{
namespace
{
  PixelData try_scale_cursor(PixelData cursor, int scaleX, int scaleY)
  {
    if(scaleX > 0 && scaleY > 0) {
      cursor.resize(scaleX, scaleY, Color::TRANSPARENT, PixelData::EResizeMode::SCALE);
    }

    return cursor;
  }

  PixelData import_image(const std::string& filename, int scaleX, int scaleY)
  {
    int width, height, n;
    STBIImageDataHolder data(stbi_load(filename.c_str(),
                                       &width,
                                       &height,
                                       &n,
                                       0));
    if(!data.getData()) {
      return PixelData(0, 0, Color::TRANSPARENT);
    }

    PixelData ret(width, height);

    auto* src_ptr = data.getData();
    auto* dst_ptr = ret.getData();
    for(int y = 0; y < height; ++y) {
      for(int x = 0; x < width; ++x) {
        std::memcpy(dst_ptr, src_ptr, n);
        dst_ptr += 4;
        src_ptr += n;
      }
    }

    return try_scale_cursor(ret, scaleX, scaleY);
  }
}
  CursorToolBase::CursorToolBase(Cursor::ECursorType cursorShape) :
    m_cursor(cursorShape)
  {
  }

  CursorToolBase::CursorToolBase(const std::string& cursorLocation, bool autoScale) :
    CursorToolBase(import_image(cursorLocation, -1, -1), autoScale)
  {
  }

  CursorToolBase::CursorToolBase(const std::string& cursorLocation, int scaleX, int scaleY, bool autoScale) :
    CursorToolBase(import_image(cursorLocation, scaleX, scaleY), autoScale)
  {
  }

  CursorToolBase::CursorToolBase(const PixelData& cursor, bool autoScale) :
    m_cursor(cursor, autoScale)
  {
    if(!m_cursor.getCursor().getWidth() ||
       !m_cursor.getCursor().getHeight()) {
      m_cursor = Cursor(Cursor::ECursorType::ECT_ARROW);
    }
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
