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
#include "selectionmoveaction.h"

#include "../manager/selectionmanager.h"

namespace pixpaint
{
  SelectionMoveAction::SelectionMoveAction(const Point& oldPosition,
                                           const Point& newPosition) :
    HistoryAction("Move Selection"),
    m_oldPosition(oldPosition),
    m_position(newPosition)
  {
  }

  SelectionMoveAction::EActionResult SelectionMoveAction::undo()
  {
    auto& selection_manager = getSelectionManager();
    selection_manager.moveSelectionRect(m_oldPosition, true, false);

    return EAR_REFRESH_IMAGE;
  }

  SelectionMoveAction::EActionResult SelectionMoveAction::redo()
  {
    auto& selection_manager = getSelectionManager();
    selection_manager.moveSelectionRect(m_position, true, false);

    return EAR_REFRESH_IMAGE;
  }
}
