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
#include "../os_specific_headers.h"
#if defined(LINUX_VERSION)
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(WINDOWS_VERSION)
#include <Shlobj.h>
#endif // defined(LINUX_VERSION)

namespace pixpaint
{
namespace settings_file_utils
{
  void tryCreateSettingsFilename()
  {
    auto settings_dir = getSettingsLocation();
    if(!os_specific::filesystem::exists(settings_dir)) {
      os_specific::filesystem::create_directory(settings_dir);
    }
  }

  std::string getSettingsFilename()
  {
    static const std::string cfg_filename = "settings.cfg";

    return (os_specific::filesystem::path(getSettingsLocation()) /
            os_specific::filesystem::path(cfg_filename)).string();
  }

  std::string getLogFilename()
  {
    static const std::string log_filename = "log.txt";

    return (os_specific::filesystem::path(getSettingsLocation()) /
            os_specific::filesystem::path(log_filename)).string();
  }

  std::string getSettingsLocation()
  {
    static const std::string cfg_filename_location = ".pixpaint";
    std::string cfg_location;

#if defined(LINUX_VERSION)
    cfg_location = getpwuid(getuid())->pw_dir;
#elif defined(WINDOWS_VERSION)
    WCHAR wcfg_location[MAX_PATH];
    if(SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, wcfg_location))) {
      char wcfg_location_cstr[MAX_PATH];
      snprintf(&wcfg_location_cstr[0], MAX_PATH, "%ls", &wcfg_location[0]);
      cfg_location = std::string(&wcfg_location_cstr[0], strnlen(&wcfg_location_cstr[0], MAX_PATH));
    }
#endif // defined(LINUX_VERSION)

    return (os_specific::filesystem::path(cfg_location) /
            os_specific::filesystem::path(cfg_filename_location)).string();
  }
}
}
