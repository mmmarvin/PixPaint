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
#include "dummyselectionmanager.h"

#include "../env/guienvironment.h"
#include "../image/rect.h"
#include "../type.h"
#include "../window/zoomablescrollarea.h"

namespace pixpaint
{
  DummySelectionManager::DummySelectionManager() :
    m_flagSelectionRect(false)
  {
  }

  void DummySelectionManager::setSelectionRect(position_t x,
                                               position_t y,
                                               position_t width,
                                               position_t height)
  {
    setSelectionRect(IntRect(x, y, width, height));
  }

  void DummySelectionManager::setSelectionRect(const IntRect& rect)
  {
    m_rect = rect;
    m_flagSelectionRect = true;

    getImageEnvironment().getScrollArea().updateDummySelectionWidget();
  }

  void DummySelectionManager::clear()
  {
    m_flagSelectionRect = false;
    getImageEnvironment().getScrollArea().updateDummySelectionWidget();
  }

  bool DummySelectionManager::selectionExists() const noexcept
  {
    return m_flagSelectionRect;
  }

  const IntRect& DummySelectionManager::getSelectionRect() const noexcept
  {
    return m_rect;
  }

  PIXPAINT_SINGLETON_FUNC_DEF(DummySelectionManager)
}
