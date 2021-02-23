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
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include "document/animationdocument.h"
#include "history/history.h"

namespace pixpaint
{
  class Animation;
  class AnimationFileTypeHandlerBase;

  class Document
  {
  public:
    Document(position_t width, position_t height, std::string filename = "");

    Animation& getAnimation() noexcept;
    const Animation& getAnimation() const noexcept;

    AnimationDocument& getAnimationDocument() noexcept;
    const AnimationDocument& getAnimationDocument() const noexcept;

    void setFilename(std::string filename);
    const std::string& getFilename() const noexcept;

    void setFileTypeInformation(AnimationFileTypeHandlerBase* fileTypeInformation);
    AnimationFileTypeHandlerBase* getFileTypeInformation() const noexcept;

    History& getHistory() noexcept;
    const History& getHistory() const noexcept;

  private:
    AnimationFileTypeHandlerBase* m_fileTypeInformation;

    AnimationDocument             m_animationDocument;
    std::string                   m_filename;

    History                       m_history;
  };
}

#endif // DOCUMENT_H
