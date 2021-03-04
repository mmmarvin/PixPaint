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
#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include "image.h"

namespace pixpaint
{
  class Animation
  {
  public:
    Animation();
    Animation(const Animation& rhs);
    Animation& operator=(const Animation& rhs);
    Animation(Animation&& rhs) = default;
    Animation& operator=(Animation&& rhs) = default;

    void addFrame(std::size_t index, Image&& frame, int duration = 1000);
    void addFrame(std::size_t index, position_t width, position_t height, int duration = 1000);
    void removeFrame(std::size_t index);
    void moveFrame(size_t srcIndex, size_t dstIndex);

    Image& getFrame(std::size_t index);
    const Image& getFrame(std::size_t index) const;
    Image& getCurrentFrame();
    const Image& getCurrentFrame() const;

    void setFrameDuration(std::size_t index, int duration);
    void setCurrentFrameDuration(int duration);
    int getFrameDuration(std::size_t index) const;
    int getCurrentFrameDuration() const;

    void setCurrentFrameIndex(std::size_t index);
    std::size_t getCurrentFrameIndex() const;
    std::size_t getFrameCount() const;

  private:
    std::vector<std::pair<std::unique_ptr<Image>, int>> m_frames;
    std::size_t                                         m_currentFrameIndex;
  };
}

#endif // ANIMATION_H
