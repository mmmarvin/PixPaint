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
#include "moveframeaction.h"

#include "../image/animation.h"
#include "../manager/documentmanager.h"
#include "../document.h"

namespace pixpaint
{
  MoveFrameAction::MoveFrameAction(size_t from, size_t to) :
    HistoryAction("Move Frame"),
    m_from(from),
    m_to(to)
  {
  }

  MoveFrameAction::EActionResult MoveFrameAction::undo()
  {
    auto& animation = getDocumentManager().getDocument().getAnimation();
    animation.moveFrame(m_to, m_from);
    animation.setCurrentFrameIndex(m_from);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_RECREATE_FRAME_LIST);
  }

  MoveFrameAction::EActionResult MoveFrameAction::redo()
  {
    auto& animation = getDocumentManager().getDocument().getAnimation();
    animation.moveFrame(m_from, m_to);
    animation.setCurrentFrameIndex(m_to);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_RECREATE_FRAME_LIST);
  }
}
