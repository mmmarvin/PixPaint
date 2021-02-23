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
#include "animationresizeaction.h"

#include "../image/animation.h"
#include "../manager/documentmanager.h"
#include "../assert.h"
#include "../document.h"

namespace pixpaint
{
  AnimationResizeAction::AnimationResizeAction(const Animation& animation) :
    HistoryAction("Resize"),
    m_animation(std::make_unique<Animation>(animation))
  {
  }

  AnimationResizeAction::EActionResult AnimationResizeAction::undo()
  {
    auto& document = getDocumentManager().getDocument();
    PIXPAINT_ASSERT(m_animation->getFrameCount() == document.getAnimation().getFrameCount(),
                    "Animations do not match");

    auto temp = document.getAnimation();
    auto& animation = document.getAnimation();
    for(size_t i = 0, isize = document.getAnimation().getFrameCount(); i < isize; ++i) {
      // must copy each frame individually, instead of copying the whole animation, since the
      // preview mananager keeps reference for each frames, and copying the whole animation will
      // invalidate those references
      animation.getFrame(i) = m_animation->getFrame(i);
    }

    m_animation = std::make_unique<Animation>(temp);

    return EActionResult::EAR_REFRESH_IMAGE_SIZE;
  }

  AnimationResizeAction::EActionResult AnimationResizeAction::redo()
  {
    return undo();
  }
}
