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
#include "animation.h"

namespace pixpaint
{
  Animation::Animation() :
    m_currentFrameIndex(0)
  {
  }

  Animation::Animation(const Animation& rhs) :
    m_currentFrameIndex(rhs.m_currentFrameIndex)
  {
    m_frames.reserve(rhs.m_frames.size());
    for(const auto& t : rhs.m_frames) {
      const auto& i = t.first;
      auto d = t.second;

      m_frames.push_back(std::make_pair(std::make_unique<Image>(*i.get()), d));
    }
  }

  Animation& Animation::operator=(const Animation& rhs)
  {
    Animation temp(rhs);
    std::swap(m_frames, temp.m_frames);
    std::swap(m_currentFrameIndex, temp.m_currentFrameIndex);

    return *this;
  }

  void Animation::addFrame(std::size_t index, Image&& frame, int duration)
  {
    m_frames.insert(m_frames.begin() + index,
                    std::make_pair(std::make_unique<Image>(std::move(frame)), duration));
  }

  void Animation::addFrame(std::size_t index, position_t width, position_t height, int duration)
  {
    m_frames.insert(m_frames.begin() + index,
                    std::make_pair(std::make_unique<Image>(width, height), duration));
  }

  void Animation::removeFrame(std::size_t index)
  {
    m_frames.erase(m_frames.begin() + index);
  }

  Image& Animation::getFrame(std::size_t index)
  {
    return *m_frames[index].first.get();
  }

  const Image& Animation::getFrame(std::size_t index) const
  {
    return *m_frames[index].first.get();
  }

  Image& Animation::getCurrentFrame()
  {
    return *m_frames[m_currentFrameIndex].first.get();
  }

  const Image& Animation::getCurrentFrame() const
  {
    return *m_frames[m_currentFrameIndex].first.get();
  }

  void Animation::setFrameDuration(std::size_t index, int duration)
  {
    m_frames[index].second = duration;
  }

  void Animation::setCurrentFrameDuration(int duration)
  {
    m_frames[m_currentFrameIndex].second = duration;
  }

  int Animation::getFrameDuration(std::size_t index) const
  {
    return m_frames[index].second;
  }

  int Animation::getCurrentFrameDuration() const
  {
    return m_frames[m_currentFrameIndex].second;
  }

  void Animation::setCurrentFrameIndex(std::size_t index)
  {
    m_currentFrameIndex = index;
  }

  std::size_t Animation::getCurrentFrameIndex() const
  {
    return m_currentFrameIndex;
  }

  std::size_t Animation::getFrameCount() const
  {
    return m_frames.size();
  }
}
