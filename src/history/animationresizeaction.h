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
#ifndef ANIMATIONRESIZEACTION_H
#define ANIMATIONRESIZEACTION_H

#include <memory>
#include "historyaction.h"

namespace pixpaint
{
  class Animation;

  class AnimationResizeAction : public HistoryAction
  {
  public:
    explicit AnimationResizeAction(const Animation& animation);

    EActionResult undo() override;
    EActionResult redo() override;

  private:
    std::unique_ptr<Animation> m_animation;
  };
}

#endif // ANIMATIONRESIZEACTION_H
