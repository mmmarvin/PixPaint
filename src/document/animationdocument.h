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
#ifndef ANIMATIONDOCUMENT_H
#define ANIMATIONDOCUMENT_H

#include <cstddef>
#include "../image/animation.h"

namespace pixpaint
{
  // TODO: Remove this altogether
  class AnimationDocument
  {
  public:
    AnimationDocument();

    Animation& getAnimation() noexcept;
    const Animation& getAnimation() const noexcept;

//    void setCurrentFrameIndex(std::size_t index);
//    std::size_t getCurrentFrameIndex() const noexcept;

  private:
    Animation   m_animation;
//    std::size_t m_currentFrameIndex;
  };
}

#endif // ANIMATIONDOCUMENT_H
