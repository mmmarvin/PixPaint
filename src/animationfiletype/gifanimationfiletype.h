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
#ifndef GIFANIMATIONFILETYPE_H
#define GIFANIMATIONFILETYPE_H

#include "animationfiletypebase.h"

namespace pixpaint
{
  class GIFAnimationFileType : public AnimationFileTypeBase
  {
  public:
    GIFAnimationFileType() {}

    bool canReadHeader(HeaderStream& stream) const override;
    int saveAnimation(const std::string& filename, const Animation& animation) override;
    int openAnimation(const std::string& filename, Animation& animation) const override;
  };
}

#endif // GIFANIMATIONFILETYPE_H
