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
#ifndef PAINTTOOLMANAGER_H
#define PAINTTOOLMANAGER_H

#include <cstddef>
#include "../pattern/observer.h"
#include "../pattern/singleton.h"

namespace pixpaint
{
  class PaintToolHandlerBase;

  class PaintToolManager : public patterns::Subject
  {
    PaintToolManager() noexcept;

  public:
    void setCurrentTool(size_t index) noexcept;
    void setToPreviousTool() noexcept;

    bool currentToolSet() const noexcept;
    bool previousToolSet() const noexcept;
    PaintToolHandlerBase& getCurrentTool() noexcept;
    const PaintToolHandlerBase& getCurrentTool() const noexcept;
    std::make_signed_t<size_t> getCurrentToolIndex() const noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(PaintToolManager)
    friend class GlobalKeyReader;
    friend class LeftToolbox;

    std::make_signed_t<size_t> m_currentToolIndex;
    std::make_signed_t<size_t> m_previousToolIndex;
  };

  PIXPAINT_SINGLETON_FUNC(PaintToolManager)
}

#endif // PAINTTOOLMANAGER_H
