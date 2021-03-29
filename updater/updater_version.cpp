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
#include "updater_version.h"

namespace pixpaint_updater
{
  std::string getAppAndVersion()
  {
    return getApp() + getVersion();
  }

  std::string getApp()
  {
    return APP_TITLE;
  }

  std::string getVersion()
  {
    auto ver = std::string(" v.") +
               std::to_string(APP_VERSION_MAJOR) +
               std::string(".") +
               std::to_string(APP_VERSION_MINOR) +
               std::to_string(APP_VERSION_PATCH);

    return ver;
  }

  int getVersionID()
  {
    return (APP_VERSION_MAJOR * 100) +
           (APP_VERSION_MINOR * 10) +
           (APP_VERSION_PATCH);
  }
}
