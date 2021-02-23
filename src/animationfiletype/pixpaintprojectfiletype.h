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
#ifndef PIXPAINTPROJECTFILETYPE_H
#define PIXPAINTPROJECTFILETYPE_H

#include "animationfiletypebase.h"

namespace pixpaint
{
  class PixPaintProjectFileType : public AnimationFileTypeBase
  {
    static constexpr std::size_t NUMBER_OF_HEADER_BITS = 6;
    static const unsigned char HEADER_VALUE[NUMBER_OF_HEADER_BITS];

#pragma pack(push, 1)
    struct ProjectHeader
    {
      std::array<unsigned char, NUMBER_OF_HEADER_BITS> header;
      std::int32_t num_frames;
    };
#pragma pack(pop)
#pragma pack(push, 1)
    struct FrameHeader
    {
      std::int32_t width;
      std::int32_t height;
    };
#pragma pack(pop)

    static_assert(sizeof(ProjectHeader) == (NUMBER_OF_HEADER_BITS + sizeof(std::int32_t)), "Invalid size of ProjectHeader. Is there padding?");
    static_assert(sizeof(FrameHeader) == (sizeof(std::int32_t) * 2), "Invalid size of ProjectHeader. Is there padding?");

  public:
    PixPaintProjectFileType() {}

    bool canReadHeader(HeaderStream& stream) const override;
    int saveAnimation(const std::string& filename, const Animation& animation) override;
    int openAnimation(const std::string& filename, Animation& animation) const override;
  };
}

#endif // PIXPAINTPROJECTFILETYPE_H
