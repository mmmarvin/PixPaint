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
#include "document.h"

namespace pixpaint
{
  Document::Document(position_t width, position_t height, std::string filename) :
    m_fileTypeInformation(nullptr),
    m_filename(std::move(filename))
  {
    getAnimation().addFrame(0, width, height);
  }

  Animation& Document::getAnimation() noexcept
  {
    return getAnimationDocument().getAnimation();
  }

  const Animation& Document::getAnimation() const noexcept
  {
    return getAnimationDocument().getAnimation();
  }

  AnimationDocument& Document::getAnimationDocument() noexcept
  {
    return m_animationDocument;
  }

  const AnimationDocument& Document::getAnimationDocument() const noexcept
  {
    return m_animationDocument;
  }

  void Document::setFilename(std::string filename)
  {
    m_filename = std::move(filename);
  }

  const std::string& Document::getFilename() const noexcept
  {
    return m_filename;
  }

  void Document::setFileTypeInformation(AnimationFileTypeHandlerBase* fileTypeInformation)
  {
    m_fileTypeInformation = fileTypeInformation;
  }

  AnimationFileTypeHandlerBase* Document::getFileTypeInformation() const noexcept
  {
    return m_fileTypeInformation;
  }

  History& Document::getHistory() noexcept
  {
    return m_history;
  }

  const History& Document::getHistory() const noexcept
  {
    return m_history;
  }
}
