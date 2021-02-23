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
#ifndef DEFINE_H
#define DEFINE_H

namespace pixpaint
{
  static constexpr auto CONFIG_SECTION_KEYBOARD_SHORTCUT = "KeyboardShortcuts";
  static constexpr auto CONFIG_SECTION_SETTINGS = "Settings";
  static constexpr auto CONFIG_SECTION_RECENT_FILES = "RecentFiles";

  static constexpr auto NUMBER_OF_RECENT_FILES = 5;

  static constexpr auto DEFAULT_IMAGE_WIDTH = 64;
  static constexpr auto DEFAULT_IMAGE_HEIGHT = 64;

  static constexpr auto DEFAULT_TOOL_ANTIALIASING = false;
  static constexpr auto DEFAULT_TEXT_ANTIALIASING = false;

  static constexpr auto MAX_CONSOLE_LINES = 100;
}

#endif // DEFINE_H
