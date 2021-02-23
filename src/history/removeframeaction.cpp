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
#include "removeframeaction.h"

#include "../image/animation.h"
#include "../manager/documentmanager.h"
#include "../document.h"

namespace pixpaint
{
  RemoveFrameAction::RemoveFrameAction(size_t frameIndex) :
    HistoryAction("Remove Frame"),
    m_index(frameIndex)
  {
    auto& animation = getDocumentManager().getDocument().getAnimation();
    m_frame = std::make_unique<Image>(animation.getFrame(frameIndex));
    m_duration = animation.getFrameDuration(frameIndex);
  }

  RemoveFrameAction::EActionResult RemoveFrameAction::undo()
  {
    auto& animation = getDocumentManager().getDocument().getAnimation();
    animation.addFrame(m_index, m_frame->getWidth(), m_frame->getHeight(), m_duration);
    animation.getFrame(m_index) = std::move(*m_frame.get());
    animation.setCurrentFrameIndex(m_index);
    m_frame.reset(nullptr);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_REFRESH_FRAME_LIST);
  }

  RemoveFrameAction::EActionResult RemoveFrameAction::redo()
  {
    auto& animation = getDocumentManager().getDocument().getAnimation();
    m_frame = std::make_unique<Image>(animation.getFrame(m_index));
    m_duration = animation.getFrameDuration(m_index);
    animation.removeFrame(m_index);

    if(m_index > 0) {
      animation.setCurrentFrameIndex(m_index - 1);
    } else {
      animation.setCurrentFrameIndex(0);
    }

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_REFRESH_FRAME_LIST);
  }
}
