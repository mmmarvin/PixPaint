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
#include "settings_file_utility.h"

#include <filesystem>
#include <string.h>
#include "../macro.h"
#include "../os_specific_functions.h"
#include "../version_specific_headers.h"

namespace pixpaint
{
namespace settings_file_utils
{
  void tryCreateSettingsFilename()
  {
    auto settings_dir = getSettingsLocation();
    if(!version_specific::filesystem::exists(settings_dir)) {
      version_specific::filesystem::create_directory(settings_dir);
    }
  }

  std::string getSettingsFilename()
  {
    static const std::string cfg_filename = "settings.cfg";

    return (version_specific::filesystem::path(getSettingsLocation()) /
            version_specific::filesystem::path(cfg_filename)).string();
  }

  std::string getLogFilename()
  {
    static const std::string log_filename = "log.txt";

    return (version_specific::filesystem::path(getSettingsLocation()) /
            version_specific::filesystem::path(log_filename)).string();
  }

  std::string getSettingsLocation()
  {
    static const std::string cfg_filename_location = ".pixpaint";
    std::string cfg_location = os_specific::getHomePath();

    return (version_specific::filesystem::path(cfg_location) /
            version_specific::filesystem::path(cfg_filename_location)).string();
  }
}
}
