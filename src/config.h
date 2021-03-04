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
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace pixpaint
{
  static constexpr auto APP_TITLE = "PixPaint";
  static constexpr auto APP_VERSION_MAJOR = 1;
  static constexpr auto APP_VERSION_MINOR = 1;
  static constexpr auto APP_VERSION_PATCH = 0;
  static constexpr auto APP_VERSION_RELEASE = 0;
  static constexpr auto APP_RELEASE_TYPE = "a";

  std::string getAppAndVersion();
  std::string getApp();
  std::string getVersion();
}

#endif // CONFIG_H
