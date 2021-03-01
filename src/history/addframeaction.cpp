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
#include "addframeaction.h"

#include "../image/image.h"
#include "../manager/documentmanager.h"
#include "../assert.h"
#include "../document.h"

namespace pixpaint
{
  AddFrameAction::AddFrameAction(size_t frameIndex) :
    HistoryAction("Add Frame"),
    m_index(frameIndex),
    m_duration(0)
  {
  }

  AddFrameAction::EActionResult AddFrameAction::undo()
  {
    auto& animation = getDocumentManager().getDocument().getAnimation();

    auto& l = animation.getFrame(m_index);
    m_duration = animation.getFrameDuration(m_index);
    m_frame = std::make_unique<Image>(std::move(l));
    animation.removeFrame(m_index);

    if(m_index > 0) {
      animation.setCurrentFrameIndex(m_index - 1);
    } else {
      animation.setCurrentFrameIndex(0);
    }

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE |
                           EActionResult::EAR_RECREATE_FRAME_LIST);
  }

  AddFrameAction::EActionResult AddFrameAction::redo()
  {
    auto& animation = getDocumentManager().getDocument().getAnimation();
    animation.addFrame(m_index, m_frame->getWidth(), m_frame->getHeight(), m_duration);
    animation.getFrame(m_index) = std::move(*m_frame.get());
    animation.setCurrentFrameIndex(m_index);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE |
                           EActionResult::EAR_RECREATE_FRAME_LIST);
  }
}
