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
#ifndef FRAME_EVENTS_H
#define FRAME_EVENTS_H

#include <cinttypes>
#include <type_traits>

namespace pixpaint
{
namespace gui_events
{
  struct FrameAddEvent
  {
    std::size_t frameAddedIndex;
    std::size_t frameCount;
  };

  struct FrameRemoveEvent
  {
    std::size_t frameRemovedIndex;
    std::size_t frameCount;
  };

  struct FrameChangeEvent
  {
    std::size_t fromFrameIndex;
    std::size_t toFrameIndex;
  };
}
}

#endif // FRAME_EVENTS_H
