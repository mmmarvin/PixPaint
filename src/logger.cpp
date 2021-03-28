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
#include "logger.h"

#include <chrono>
#include <ctime>
#include "utility/settings_file_utility.h"
#include "version.h"
#include "debug_log.h"
#include "version_specific_headers.h"

namespace pixpaint
{
  Logger::Logger()
  {
    settings_file_utils::tryCreateSettingsFilename();
    if(!version_specific::filesystem::exists(settings_file_utils::getLogFilename())) {
      m_out.open(settings_file_utils::getLogFilename(), std::ios_base::out);
      if(m_out.is_open()) {
        m_out << getAppAndVersion()
              << " Log File\n"
              << "=========================\n"
              << std::endl;
      }
    } else {
      m_out.open(settings_file_utils::getLogFilename(), std::ios_base::out);
    }
  }

  void Logger::write(std::string msg)
  {
    auto remove_newline_and_space = [](std::string s) {
      while(s.size() &&
            (isspace(s.back()) || s.back() == '\n')) {
        s.pop_back();
      }

      return s;
    };

    if(m_out.is_open()) {
      auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      auto current_time = std::string(std::ctime(&t));
      m_out << remove_newline_and_space(current_time) << ": " << msg << std::endl;
    }
  }

  PIXPAINT_SINGLETON_FUNC_DEF(Logger)
}
