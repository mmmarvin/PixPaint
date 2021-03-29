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
#include "os_specific_functions.h"

#include <boost/process.hpp>
#include "macro.h"
#if defined(LINUX_VERSION)
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(WINDOWS_VERSION)
#include <Shlobj.h>
#endif // defined(LINUX_VERSION)

namespace pixpaint
{
  namespace os_specific
  {
    bool callProcess(const std::string& process_name,
                     const std::string& process_parameters)
    {
      if(process_parameters.empty()) {
        try {
          boost::process::child c(process_name);

          if(c.running()) {
            c.detach();
            return true;
          }
        } catch(...) {}

        return false;
      }

      try {
        boost::process::child c(process_name, process_parameters);

        if(c.running()) {
          c.detach();
          return true;
        }
      } catch(...) {}

      return false;
    }

    std::string getHomePath()
    {
      std::string ret;
#if defined(LINUX_VERSION)
      ret = getpwuid(getuid())->pw_dir;
#elif defined(WINDOWS_VERSION)
      WCHAR home_location[MAX_PATH];
      if(SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, home_location))) {
        char home_location_cstr[MAX_PATH];
        snprintf(&home_location_cstr[0], MAX_PATH, "%ls", &home_location[0]);
        ret = std::string(&home_location_cstr[0], strnlen(&home_location_cstr[0], MAX_PATH));
      } else {
        ret = ".";
      }
#endif // defined(LINUX_VERSION)
      return ret;
    }
  }
}
