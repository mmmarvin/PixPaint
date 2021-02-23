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
#ifndef DRAWERACTION_H
#define DRAWERACTION_H

#include <vector>
#include "../drawer/drawer.h"
#include "../image/pixeldata.h"
#include "../image/rect.h"
#include "historyaction.h"

namespace pixpaint
{
  class DrawerAction : public HistoryAction
  {
  public:
    DrawerAction(std::string name, std::vector<std::unique_ptr<Drawer::DrawerCommand>> flushedRects);

    EActionResult undo() override final;
    EActionResult redo() override final;

  private:
    std::vector<std::unique_ptr<Drawer::DrawerCommand>> m_flushedRects;
  };
}

#endif // DRAWERACTION_H
