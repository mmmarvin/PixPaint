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
#ifndef DUMMYSELECTIONMANAGER_H
#define DUMMYSELECTIONMANAGER_H

#include "../image/rect.h"
#include "../pattern/singleton.h"

namespace pixpaint
{
  class DummySelectionManager
  {
    PIXPAINT_SINGLETON_FUNC_FRIEND(DummySelectionManager)
    DummySelectionManager();

  public:
    void setSelectionRect(position_t x,
                          position_t y,
                          position_t width,
                          position_t height);
    void setSelectionRect(const IntRect& rect);
    void clear();

    bool selectionExists() const noexcept;
    const IntRect& getSelectionRect() const noexcept;

  private:
    IntRect m_rect;
    bool    m_flagSelectionRect;
  };

  PIXPAINT_SINGLETON_FUNC(DummySelectionManager)
}

#endif // DUMMYSELECTIONMANAGER_H
