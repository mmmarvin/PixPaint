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
#include "animationdocument.h"

namespace pixpaint
{
  AnimationDocument::AnimationDocument() /*:
    m_currentFrameIndex(0)*/
  {
  }

  Animation& AnimationDocument::getAnimation() noexcept
  {
    return m_animation;
  }

  const Animation& AnimationDocument::getAnimation() const noexcept
  {
    return m_animation;
  }

//  void AnimationDocument::setCurrentFrameIndex(std::size_t index)
//  {
//    m_currentFrameIndex = index;
//  }

//  std::size_t AnimationDocument::getCurrentFrameIndex() const noexcept
//  {
//    return m_currentFrameIndex;
//  }
}
