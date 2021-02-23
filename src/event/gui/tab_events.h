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
#ifndef TAB_EVENTS_H
#define TAB_EVENTS_H

#include <cinttypes>

namespace pixpaint
{
namespace gui_events
{
  // Event emitted after the tab is removed
  struct TabAddEvent
  {
    std::size_t numberOfTabs;
  };

  // Event emitted before the tab is actually removed
  struct TabRemoveEvent
  {
    std::size_t numberOfTabs;
  };

  // Event emitted after the tab has changed
  struct TabChangeEvent
  {
    int fromTabIndex;
    int toTabIndex;
  };
}
}

#endif // TAB_EVENTS_H
